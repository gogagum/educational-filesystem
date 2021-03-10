#ifndef DATA_GETTERS_H
#define DATA_GETTERS_H

#include <string.h>
#include "fs.h" // structs

/*
 * Fills fs data struct from pointer
 */
void get_data(struct fs_data* filesys_data, void* mapped_file);

/*
 * Gets inode by index.
 */
void get_inode(size_t i, const struct fs_data* filesys_data,
               void* mapped_file, struct inode* ret_inode);

/*
 * Gets block by index.
 */
void get_block(size_t i, const struct fs_data* filesys_data, 
               void* mapped_file, void* ret_block);


#endif  // DATA_GETTERS_H