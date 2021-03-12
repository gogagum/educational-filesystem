#include <stdio.h>
#include "menus.h"
#include "fs.h"
#include "open_n_create_fs_file.h"
#include "parse_path.h"


int 
main() 
{
    enum STARTUP_MENU_RESULT res = ERROR;

    while (res == ERROR)
    {
        res = startup_menu();
        if (res == ERROR)
        {
            printf("%s\n", "Unsupported answer.");
        }
    }

    struct fs_data filesys_data;
    void* mapped_file;
    int fd;

    switch (res)
    {
    case CREATE:
        mapped_file = create_menu(&filesys_data);
        break;
    case OPEN:
        mapped_file = open_menu(&fd, &filesys_data);
        break;
    }

    if (mapped_file == NULL) {
        return 1;
    }
    //loop(&filesys_data, mapped_file);
    return 0;
}
