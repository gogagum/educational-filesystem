#ifndef DATA_GETTERS_H
#define DATA_GETTERS_H

#include "fs.h" // structs

/*
 * Fills fs data struct from pointer
 */
void get_data(struct fs_data* file_sys_data, void* mapped_file);

/*
 * Gets inode by index.
 */
int get_inode(const struct fs_data* file_sys_data, 
              void* mapped_file, struct inode* ret_inode);

/*
 * Gets block by index
 */
int get_block(const struct fs_data* file_sys_data, 
              void* mapped_file, void* ret_ptr);


#endif  // DATA_GETTERS_H