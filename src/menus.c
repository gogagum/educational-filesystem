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
        getchar();

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
}

//----------------------------------------------------------------------------//
void* 
open_menu(int* ret_fd,
          struct fs_data* filesys_data)
{
    char* filename = NULL;
    size_t n = 0;

    if (get_path_from_user(&filename, &n) != -1)
    {
        void* mapped_file = open_fs_file(ret_fd, filename);
        free(filename);
        return mapped_file;
    }
    return NULL;
}

//----------------------------------------------------------------------------//
void*
create_menu(int* ret_fd,
            struct fs_data* filesys_data)
{
    char* filename = NULL;
    size_t n = 0;

    if (get_path_from_user(&filename, &n) != -1)
    {
        size_t inodes_cnt;
        printf("%s\n", "Choose inodes count.");
        scanf("%lx", &inodes_cnt);
        getchar();

        size_t blocks_cnt;
        printf("%s\n", "Choose blocks count.");
        scanf("%lx", &blocks_cnt);
        getchar();

#ifdef DEBUG
        printf("Inodes cnt: %li.\n", inodes_cnt);
        printf("Blocks cnt: %li.\n", blocks_cnt);
#endif

        void* mapped_file = 
            create_fs_file(ret_fd, filename, inodes_cnt, blocks_cnt);

        get_filesys_data(filesys_data, mapped_file);

        create_dir_inode(0, 
                         ""/*No matter what the name is*/, 
                         filesys_data, 
                         mapped_file);

        free(filename);
        return mapped_file;
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

    while (curr_cmd != EXIT)
    {
#ifdef DEBUG
        printf("%s", "Filesys data:");
        printf("fs_data->blocks_tail_beginning =  %li\n", filesys_data->blocks_tail_beginning);
        printf("fs_data->inodes_tail_beginning =  %li\n", filesys_data->inodes_tail_beginning);
#endif
        curr_cmd = command_choosing_menu();

        switch(curr_cmd) 
        {
            case READ:
                read_cmd(filesys_data, mapped_file);
                break;
            case WRITE:
                write_cmd(filesys_data, mapped_file);
                break;
            case MKDIR:
                makedir_cmd(filesys_data, mapped_file);
                break;
            case LS:
                ls_cmd(filesys_data, mapped_file);
                break;
            case RM:
                rm_cmd(filesys_data, mapped_file);
                break;
            case EXIT: 
                return;
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
    printf("%s\n", "# 3. MKDIR.                                       #");
    printf("%s\n", "# 4. LS.                                          #");
    printf("%s\n", "# 5. RM.                                          #");
    printf("%s\n", "# 6. EXIT.                                        #");
    printf("%s\n", "###################################################");

    char c;
    int input_res = scanf("%c", &c);
    getchar();

#ifdef DEBUG
    printf("Got command: %c.\n", c);
#endif

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
            return MKDIR;
        case '4':
            return LS;
        case '5':
            return RM;
        case '6':
            return EXIT;
    }

    return OPERATION_MENU_ERROR;
}

//----------------------------------------------------------------------------//
ssize_t
get_path_from_user(char** ret_path, 
                   size_t* n)
{
    ssize_t res;
    do {
        printf("%s\n", "Print file location.");
        res = getline(ret_path, n, stdin);
        if ((*ret_path)[0] != '/')
        {
            printf("Current location is not stored, print full path.\n");
        }
    } 
    while ((*ret_path)[0] != '/');

    res--;                    // Let res be the same as string length now.
    (*ret_path)[res] = '\0';  // After getline(...), string finishes with '\n'.

#ifdef DEBUG
    size_t str_len = strlen(*ret_path);

    printf("Path from user: \"%s\".\n", *ret_path);
    printf("Path string length: %li.\n", str_len);
    printf("res: %li.\n", res);
#endif
    
    return res;
}

//----------------------------------------------------------------------------//
void 
read_cmd(struct fs_data* filesys_data,
         void* mapped_file)
{
    char* filename = NULL;
    size_t n = 0;
    get_path_from_user(&filename, &n);

    struct inode* inode_ptr = 
        find_inode_ptr_by_name(filename, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);
    free(filename);
    size_t last_read = 1;
    off_t curr_offset = 0;
    char buff[BYTES_BLOCK_SIZE];
    do 
    {
        last_read = get_chunk(buff, 
                              inode_ptr, 
                              BYTES_BLOCK_SIZE, 
                              curr_offset, 
                              filesys_data, 
                              mapped_file);
        curr_offset += last_read;
        printf("%.*s", (int)last_read, buff);
    }
    while (last_read != 0);
}

//----------------------------------------------------------------------------//
void 
write_cmd(struct fs_data* filesys_data,
          void* mapped_file)
{
    char* filename = NULL;
    size_t n = 0;
    get_path_from_user(&filename, &n);

    struct inode* inode_ptr =  
        find_inode_ptr_by_name(filename, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);
    free(filename);
    char* str_to_write;
    printf("%s\n", "Print a string to append into file.");
    getline(&str_to_write, &n, stdin);

    size_t chars_to_write = n;

    size_t curr_offset = inode_ptr->size;
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
makedir_cmd(struct fs_data* filesys_data, 
            void* mapped_file)
{
    char* creation_path = NULL;
    size_t path_length = 0;
    get_path_from_user(&creation_path, &path_length);

    char* filename = NULL;
    size_t file_name_length = 0;
    printf("%s\n", "Print new directory name.");
    getline(&filename, &file_name_length, stdin);

    inode_idx_t to_create_inode_idx = 
        find_inode_idx_by_name(creation_path, 
                               get_root_inode_ptr(filesys_data, mapped_file),
                               filesys_data, 
                               mapped_file);
    create_dir_inode(to_create_inode_idx, filename, filesys_data, mapped_file);
}

//----------------------------------------------------------------------------//
void
ls_cmd(struct fs_data* filesys_data,
       void* mapped_file)
{
    char* path = NULL;
    size_t path_length = 0;
    get_path_from_user(&path, &path_length);
    
    size_t inode_idx = 
        find_inode_idx_by_name(path, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);
    
    struct inode* inode_ptr = get_inode_ptr(inode_idx, 
                                            filesys_data, 
                                            mapped_file);
    assert(inode_ptr->type == DIR);

    struct link ith_internal_file_link;
    size_t links_cnt = get_dir_links_cnt(inode_ptr, filesys_data, mapped_file);

#ifdef DEBUG
    printf("LS worked at inode with idx %li.\n", inode_idx);
    printf("Links cnt: %li. \n", links_cnt);
#endif


    for (size_t i = 0; i < links_cnt; ++i)
    {
        get_ith_internal_file_link(&ith_internal_file_link, 
                                   inode_ptr, 
                                   i, 
                                   filesys_data, 
                                   mapped_file);
        printf("%s\n", ith_internal_file_link.name);
    }
}

//----------------------------------------------------------------------------//
void
rm_cmd(
    struct fs_data* filesys_data,
    void* mapped_file)
{
    char* path;
    size_t path_length;
    get_path_from_user(&path, &path_length);

    size_t inode_idx = 
        find_inode_idx_by_name(path, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);

    struct inode* inode_ptr = get_inode_ptr(inode_idx, 
                                            filesys_data, 
                                            mapped_file);
    
    struct inode* parent_inode_ptr = 
        get_parent_directory_inode_ptr(inode_ptr, filesys_data, mapped_file);

    size_t parent_dir_links_cnt = get_dir_links_cnt(parent_inode_ptr, 
                                                    filesys_data, 
                                                    mapped_file);

    struct link* list_end_ptr = 
        get_ith_internal_file_link_ptr(inode_ptr, 
                                       parent_dir_links_cnt, 
                                       filesys_data, 
                                       mapped_file);
    

    struct link* link_to_delete = find_link_ptr_by_inode_idx(inode_idx, 
                                                             parent_inode_ptr, 
                                                             filesys_data, 
                                                             mapped_file);    

    if (link_to_delete != list_end_ptr)
    {
        memcpy(link_to_delete, list_end_ptr, sizeof(struct link));
    }

    set_dir_links_cnt(parent_dir_links_cnt - 1, 
                      parent_inode_ptr,
                      filesys_data, 
                      mapped_file);

    remove_inode(inode_ptr, filesys_data, mapped_file);
}
