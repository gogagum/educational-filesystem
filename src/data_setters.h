#ifndef DATA_SETTERS_H
#define DATA_SETTERS_H

#include <string.h>
#include "fs.h"

/*
 * Copies filesys_data to the beginning of the file.
 */
void 
set_filesys_data(const struct fs_data* filesys_data, void* mapped_file);

/*
 * Sets inode by index.
 */
void 
set_inode(size_t i, const struct fs_data* filesys_data, void* mapped_file, 
          const struct inode* inode_to_set);

/*
 * Sets block by index.
 */
void 
set_block(size_t i, const struct fs_data* filesys_data, void* mapped_file, 
          const void* block_to_set);


#endif // DATA_SETTERS_H