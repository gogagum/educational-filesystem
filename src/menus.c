#include "menus.h"

//----------------------------------------------------------------------------//
enum STARTUP_MENU_RESULT
startup_menu()
{
    printf("%s\n", "###################################################");
    printf("%s\n", "# Create, or open:                                #");
    printf("%s\n", "###################################################");
    printf("%s\n", "# 1. Open.                                        #");
    printf("%s\n", "# 2. Create.                                      #");
    printf("%s\n", "###################################################");

    char c;
    int input_res = scanf("%c", &c);

    if (input_res != 1)
    {
        return ERROR;
    }

    if (c == '1') 
    {
        return OPEN;
    }

    if (c == '2')
    {
        return CREATE;
    }

    return ERROR;
}

//----------------------------------------------------------------------------//
void* 
open_menu(int* ret_fd,
          struct fs_data* filesys_data)
{
    char filename[4096];

    scanf("%s", filename);

    void* mapped_file_ptr = open_fs_file(ret_fd, filename);

}

//----------------------------------------------------------------------------//
void*
create_menu(struct fs_data* filesys_data)
{
    char filename[4096];
}