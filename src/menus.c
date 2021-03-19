#include "menus.h"

//----------------------------------------------------------------------------//
enum STARTUP_MENU_RESULT
startup_menu()
{
    char c = '0';
    while (c != '1' && c != '2')
    {
        printf("%s\n", "###################################################");
        printf("%s\n", "# Create, or open:                                #");
        printf("%s\n", "###################################################");
        printf("%s\n", "# 1. Open.                                        #");
        printf("%s\n", "# 2. Create.                                      #");
        printf("%s\n", "###################################################");

        int input_res = scanf("%c", &c);

        if (c != '1' && c != '2') 
        {
            printf("\"%c\" is not a correct command.\n", c);
        }
    }

    switch (c)
    {
        case '1':
            return OPEN;
        case '2':
            return CREATE;
    }

    return STARTUP_MENU_ERROR;
}

//----------------------------------------------------------------------------//
void* 
open_menu(int* ret_fd,
          struct fs_data* filesys_data)
{
    printf("%s\n", "Print filename (full path, or relative)");

    char* filename;
    size_t n = 0;

    if (getline(&filename, &n, stdin) != -1)
    {
        void* mapper_file = open_fs_file(ret_fd, filename);
        free(filename);
        return open_fs_file(ret_fd, filename);
    }

    return NULL;
}

//----------------------------------------------------------------------------//
void*
create_menu(int* ret_fd,
            struct fs_data* filesys_data)
{
    printf("%s\n", "Print filename (full path, or relative)");
    
    char* filename;
    size_t n = 0;

    if (getline(&filename, &n, stdin) != -1)
    {
        size_t inodes_cnt;
        printf("%s\n", "Choose inodes count.");
        scanf("%lx", &inodes_cnt);

        size_t blocks_cnt;
        printf("%s\n", "Choose blocks count.");
        scanf("%lx", &blocks_cnt);

        return create_fs_file(ret_fd, filename, inodes_cnt, blocks_cnt);
    }

    return NULL;
}

//----------------------------------------------------------------------------//
void
loop(int fd, 
     struct fs_data* filesys_data,
     void* mapped_file)
{
    enum OPERATION_MENU_RESULT curr_cmd = OPERATION_MENU_ERROR;

    size_t curr_dir_inode = 0;  // start from /

    while (curr_cmd != EXIT)
    {
        curr_cmd = command_choosing_menu();

        switch(curr_cmd) 
        {
            case READ:
            case WRITE:
            case LS:
            case RM:
            case EXIT: return;
        }
    }
}

//----------------------------------------------------------------------------//
enum OPERATION_MENU_RESULT
command_choosing_menu()
{
    printf("%s\n", "###################################################");
    printf("%s\n", "# Choose command:                                 #");
    printf("%s\n", "###################################################");
    printf("%s\n", "# 1. READ.                                        #");
    printf("%s\n", "# 2. WRITE.                                       #");
    printf("%s\n", "# 3. LS.                                          #");
    printf("%s\n", "# 4. RM.                                          #");
    printf("%s\n", "# 5. EXIT.                                        #");
    printf("%s\n", "###################################################");

    char c;
    int input_res = scanf("%c", &c);

    if (input_res != 1)
    {
        return OPERATION_MENU_ERROR;
    }

    switch (c)
    {
        case '1':
            return READ;
        case '2':
            return WRITE;
        case '3':
            return LS;
        case '4':
            return RM;
        case '5':
            return EXIT;
    }

    return OPERATION_MENU_ERROR;
}

//----------------------------------------------------------------------------//
ssize_t
get_path_from_user(char** ret_path, size_t* n)
{
    do {
        printf("%s\n", "Print file location.");
        ssize_t res = getline(ret_path, n, stdin);
        if ((*ret_path)[0] != '/')
        {
            printf("Current location is not stored, print full path.\n");
        }
    } 
    while ((*ret_path)[0] != '/');
}

//----------------------------------------------------------------------------//
void 
read_cmd(struct fs_data* filesys_data,
         void* mapped_file)
{
    char* filename;
    size_t n;
    ssize_t input_res = get_path_from_user(&filename, &n);

    struct inode* inode_ptr = 
        find_inode_ptr_by_name(filename + 1, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);
    size_t last_read = 1;
    off_t curr_offset = 0;
    char buff[BYTES_BLOCK_SIZE + 1];
    while (last_read != 0) 
    {
        last_read = get_chunk(buff, 
                              inode_ptr, 
                              BYTES_BLOCK_SIZE, 
                              curr_offset, 
                              filesys_data, 
                              mapped_file);
        curr_offset += last_read;
        buff[last_read + 1] = '\0';
        printf("%s", buff);
    }
}

//----------------------------------------------------------------------------//
void 
write_cmd(struct fs_data* filesys_data,
          void* mapped_file)
{
    char* filename;
    size_t n;
    ssize_t input_res = get_path_from_user(&filename, &n);

    struct inode* inode_ptr =  
        find_inode_ptr_by_name(filename, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);
    char* str_to_write;
    printf("%s\n", "Print a string to append into file.");
    input_res = getline(&str_to_write, &n, stdin);

    size_t chars_to_write = n;

    size_t curr_offset = (*inode_ptr).size;
    grow_file(chars_to_write, inode_ptr, filesys_data, mapped_file);

    size_t last_written;

    do
    {
        last_written = set_chunk(str_to_write, 
                                 inode_ptr, 
                                 BYTES_BLOCK_SIZE, 
                                 curr_offset, 
                                 filesys_data, 
                                 mapped_file);
        chars_to_write -= last_written;
        curr_offset += last_written;
    }
    while (last_written);
}

//----------------------------------------------------------------------------//
void
ls_cmd(struct fs_data* filesys_data,
       void* mapped_file)
{
    char* path;
    size_t path_length;
    printf("%s\n", "Print directory path.");
    getline(&path, &path_length, stdin);

    size_t inode_idx = find_inode_idx_by_name(path, 
                                              get_root_inode_ptr(filesys_data, 
                                                                 mapped_file), 
                                              filesys_data, 
                                              mapped_file);
    
    struct inode* inode_ptr = get_inode_ptr(inode_idx, 
                                            filesys_data, 
                                            mapped_file);
    assert((*inode_ptr).type == DIR);

    struct link ith_internal_file_link;
    struct dir_data* curr_dir_data = get_dir_data_ptr(inode_ptr, 
                                                      filesys_data, 
                                                      mapped_file);

    for (size_t i = 0; i < (*curr_dir_data).internal_files_cnt; ++i)
    {
        get_ith_internal_file_link(&ith_internal_file_link, inode_ptr, i, filesys_data, mapped_file);
        printf("%s\n", ith_internal_file_link.name);
    }
}

//----------------------------------------------------------------------------//
void
rm_cmd(
    struct fs_data* filesys_data,
       void* mapped_file)
{

}