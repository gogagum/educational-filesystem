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

        errno = 0;
        char* buff;
        int input_res = scanf("%m[12]", &buff);
        if (input_res == 1)
        {
            c = buff[0];
            free(buff);
        }
        else if (errno != 0)
        {
            perror("scanf(...) error");
        }
        else
        {
            printf("\"%s\" is not a correct command.\n", buff);
        }

        getchar();
    }

    switch (c)
    {
        case '1':
            return OPEN;
        case '2':
            return CREATE;
        default:
            assert(false);
    }
}

//----------------------------------------------------------------------------//
void* 
open_menu(int* ret_fd,
          struct fs_data* filesys_data)
{
    char* filename = NULL;
    size_t n = 0;

    void* mapped_file;

    if (get_path_from_user(&filename, &n, "Print file location.") != -1)
    {
        mapped_file = open_fs_file(ret_fd, filename);
        free(filename);
        if (mapped_file != NULL)
        {
            get_filesys_data(filesys_data, mapped_file);
            return mapped_file;
        }
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

    if (get_path_from_user(&filename, &n, "Print file location.") != -1)
    {
        size_t inodes_cnt;
        printf("%s\n", "Choose inodes count.");
        get_size_t_from_user(&inodes_cnt, "Invalid input. Try again.");

        size_t blocks_cnt;
        printf("%s\n", "Choose blocks count.");
        get_size_t_from_user(&blocks_cnt, "Invalid input. Try again.");

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
loop(struct fs_data* filesys_data,
     void* mapped_file)
{
    enum OPERATION_MENU_RESULT curr_cmd = OPERATION_MENU_ERROR;

    while (curr_cmd != EXIT)
    {
        curr_cmd = command_choosing_menu();

        switch (curr_cmd)
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
            case TOUCH:
                touch_menu(filesys_data, mapped_file);
                break;
            case LS:
                ls_cmd(filesys_data, mapped_file);
                break;
            case RM:
                rm_cmd(filesys_data, mapped_file);
                break;
            case EXIT:
                set_filesys_data(filesys_data, mapped_file);
                break;
            default:
                assert(false);
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
    printf("%s\n", "# 4. TOUCH                                        #");
    printf("%s\n", "# 5. LS.                                          #");
    printf("%s\n", "# 6. RM.                                          #");
    printf("%s\n", "# 7. EXIT.                                        #");
    printf("%s\n", "###################################################");

    char c;
    int input_res = scanf("%c", &c);
    getchar();

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
            return TOUCH;
        case '5':
            return LS;
        case '6':
            return RM;
        case '7':
            return EXIT;
        default:
            assert(false);
    }
}

//----------------------------------------------------------------------------//
ssize_t
get_path_from_user(char** ret_path, 
                   size_t* n, 
                   char* message_str)
{
    ssize_t res;
    do {
        printf("%s\n", message_str);
        res = getline(ret_path, n, stdin);
        if ((*ret_path)[0] != '/')
        {
            printf("Current location is not stored, print full path.\n");
        }
    } 
    while ((*ret_path)[0] != '/');

    res--;                    // Let res be the same as string length now.
    (*ret_path)[res] = '\0';  // After getline(...), string finishes with '\n'.
    
    return res;
}

//----------------------------------------------------------------------------//

void
get_size_t_from_user(size_t* ret_size,
                     const char* message_str)
{
    errno = 0;
    int scan_result = scanf("%zu", ret_size);
    if (scan_result != 1)
    {
        if (errno != 0)
        {
            perror("scanf() error");
        }
        else
        {
            printf("%s\n", message_str);
        }
    }
    getchar();
}

//----------------------------------------------------------------------------//
ssize_t
get_filename_to_remove(char** ret_path,
                       size_t* n)
{
    ssize_t res;
    bool possible_input;
    do
    {
        printf("%s\n", "Print filename to remove.");
        res = getline(ret_path, n, stdin);

        possible_input =
            (strcmp(*ret_path, UP_DIR) == 0 || strcmp(*ret_path, CURR_DIR) == 0);
        if (!possible_input)
        {
            printf("%s\n", "It is impossible to remove '.' or '..'.");
        }
    } while (!possible_input);

    --res;
    return res;
}

//----------------------------------------------------------------------------//
void 
read_cmd(struct fs_data* filesys_data,
         void* mapped_file)
{
    char* filename = NULL;
    size_t n = 0;
    get_path_from_user(&filename, &n, "Print directory location.");

    struct inode* inode_ptr = 
        find_inode_ptr_by_name(filename, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);
    free(filename);
    off_t curr_offset = 0;
    char buff[BYTES_BLOCK_SIZE];

    size_t to_read = inode_ptr->size;

    do 
    {
        size_t last_read = get_chunk(buff,
                              inode_ptr, 
                              to_read, 
                              curr_offset, 
                              filesys_data, 
                              mapped_file);
        curr_offset += (off_t)last_read;
        to_read -= last_read;
        printf("%.*s", (int)last_read, buff);
    }
    while (to_read != 0);
}

//----------------------------------------------------------------------------//
void 
write_cmd(struct fs_data* filesys_data,
          void* mapped_file)
{
    char* filename = NULL;
    size_t n = 0;
    get_path_from_user(&filename, &n, "Print directory location.");

    struct inode* inode_ptr =  
        find_inode_ptr_by_name(filename, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);
    free(filename);
    char* str_to_write = NULL;
    n = 0;
    printf("%s\n", "Print a string to append into file.");
    ssize_t chars_to_write = getline(&str_to_write, &n, stdin);

    off_t curr_offset = (off_t)inode_ptr->size;
    grow_file(chars_to_write, inode_ptr, filesys_data, mapped_file);

    size_t last_written;

    do
    {
        last_written = set_chunk(str_to_write + curr_offset, 
                                 inode_ptr, 
                                 chars_to_write, 
                                 curr_offset,
                                 filesys_data, 
                                 mapped_file);
        chars_to_write -= (off_t)last_written;
        curr_offset += (off_t)last_written;
    }
    while (chars_to_write);
}

//----------------------------------------------------------------------------//
void
makedir_cmd(struct fs_data* filesys_data, 
            void* mapped_file)
{
    char* creation_path = NULL;
    size_t path_length = 0;
    get_path_from_user(&creation_path, 
                       &path_length, 
                       "Print directory location (where to create).");

    char* filename = NULL;
    size_t file_name_length = 0;
    printf("%s\n", "Print new directory name.");
    ssize_t res = getline(&filename, &file_name_length, stdin);

    --res;
    filename[res] = '\0';

    inode_idx_t creation_inode_idx = 
        find_inode_idx_by_name(creation_path, 
                               get_root_inode_ptr(filesys_data, mapped_file),
                               filesys_data, 
                               mapped_file);
    create_dir_inode(creation_inode_idx, filename, filesys_data, mapped_file);
}

//----------------------------------------------------------------------------//
void
touch_menu(struct fs_data* filesys_data,
           void* mapped_file)
{
    char* creation_path = NULL;
    size_t path_length = 0;
    get_path_from_user(&creation_path, 
                       &path_length, 
                       "Print directory location (where to create).");

    char* filename = NULL;
    size_t file_name_length = 0;
    printf("%s\n", "Print new directory name.");
    ssize_t res = getline(&filename, &file_name_length, stdin);

    --res;
    filename[res] = '\0';

    inode_idx_t creation_inode_idx = 
        find_inode_idx_by_name(creation_path, 
                               get_root_inode_ptr(filesys_data, mapped_file),
                               filesys_data, 
                               mapped_file);

    create_reg_inode(filename, creation_inode_idx, filesys_data, mapped_file);
}

//----------------------------------------------------------------------------//
void
ls_cmd(struct fs_data* filesys_data,
       void* mapped_file)
{
    char* path = NULL;
    size_t path_length = 0;
    get_path_from_user(&path, &path_length, "Print directory location.");
    
    size_t inode_idx = 
        find_inode_idx_by_name(path, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);
    
    struct inode* inode_ptr =
        get_inode_ptr(inode_idx, filesys_data, mapped_file);
    assert(inode_ptr->type == DIR);

    struct link ith_internal_file_link;
    size_t links_cnt = get_dir_links_cnt(inode_ptr, filesys_data, mapped_file);

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
    char* path = NULL;
    size_t path_length = 0;
    get_path_from_user(&path, &path_length, "Print directory where file is.");
    struct inode* parent_inode_ptr = 
        find_inode_ptr_by_name(path, 
                               get_root_inode_ptr(filesys_data, mapped_file), 
                               filesys_data, 
                               mapped_file);

    char* filename = NULL;
    size_t filename_length = 0;
    ssize_t res = get_filename_to_remove(&filename, &filename_length);
    filename[res] = '\0';

    struct inode* inode_ptr = find_internal_inode_ptr_by_name(filename,
                                                              parent_inode_ptr,
                                                              filesys_data,
                                                              mapped_file);

    size_t parent_dir_links_cnt = get_dir_links_cnt(parent_inode_ptr, 
                                                    filesys_data, 
                                                    mapped_file);

    // link to move to the freed place
    struct link* list_end_ptr = 
        get_ith_internal_file_link_ptr(parent_inode_ptr, 
                                       parent_dir_links_cnt - 1, 
                                       filesys_data, 
                                       mapped_file);
    

    struct link* link_to_delete = 
        find_link_ptr_by_inode_idx(get_inode_idx_by_ptr(inode_ptr, 
                                                        filesys_data, 
                                                        mapped_file),
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
