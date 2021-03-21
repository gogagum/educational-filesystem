#include <stdio.h>
#include <error.h>
#include "menus.h"
#include "fs.h"
#include "open_n_create_fs_file.h"
#include "parse_path.h"


int 
main() 
{
    enum STARTUP_MENU_RESULT res = STARTUP_MENU_ERROR;

    while (res == STARTUP_MENU_ERROR)
    {
        res = startup_menu();
        if (res == STARTUP_MENU_ERROR)
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
        mapped_file = create_menu(&fd, &filesys_data);
        break;
    case OPEN:
        mapped_file = open_menu(&fd, &filesys_data);
        break;
    }

    if (mapped_file == NULL) {
        error(0, 0, "%s", "Could not map file.");
        return 1;
    }
    
    loop(fd, &filesys_data, mapped_file);

    munmap(mapped_file, 
           sizeof(struct fs_data) + 
           filesys_data.blocks_cnt * BYTES_BLOCK_SIZE + 
           filesys_data.inodes_cnt * sizeof(struct inode));
    close(fd);

    return 0;
}
