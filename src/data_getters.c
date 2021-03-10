#include "data_getters.h"

/*
 * Fills fs data struct from pointer
 */
void get_data(struct fs_data* file_sys_data, void* mapped_file)
{
    memcpy((void*)file_sys_data, mapped_file, sizeof(struct fs_data));
}

/*
 * Gets inode by index.
 */
int get_inode(const struct fs_data* file_sys_data, 
              void* mapped_file, struct inode* ret_inode)
{
    // To be implemented
}