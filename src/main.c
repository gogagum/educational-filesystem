#include <stdio.h>
#include <error.h>
#include "menus.h"
#include "fs.h"
#include "open_n_create_fs_file.h"


int 
main() 
{
#ifdef DEBUG
    printf("%s\n", "DEBUG indicator");

    printf("sizeof(struct link): %li.\n", sizeof(struct link));
    printf("sizeof(struct inode): %li.\n", sizeof(struct inode));
    printf("sizeof(struct fs_data): %li.\n", sizeof(struct fs_data));
#endif // DEBUG
    
    enum STARTUP_MENU_RESULT res = startup_menu();

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
    default:
        assert(false);
    }

    if (mapped_file == NULL) {
        error(0, 0, "%s", "Could not mmap file.");
    }
    
    loop(&filesys_data, mapped_file);

    munmap(mapped_file, 
           BLOCKS_INFO_SECTION_SIZE * BYTES_BLOCK_SIZE + 
           filesys_data.blocks_cnt * BYTES_BLOCK_SIZE + 
           filesys_data.inodes_cnt * sizeof(struct inode));
    close(fd);

    return 0;
}
