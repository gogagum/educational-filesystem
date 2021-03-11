#ifndef DATA_GETTERS_H
#define DATA_GETTERS_H

#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "fs.h" // structs

/*
 * Fills fs data struct from pointer
 */
void 
get_filesys_data(struct fs_data* filesys_data, 
                 const void* mapped_file);

/*
 * Gets inode pointer by index.
 */
struct inode*
get_inode_ptr(size_t inode_index,
              const struct fs_data* filesys_data,
              void* mapped_file);

/*
 * Gets inode by index.
 */
void 
get_inode(size_t inode_index, 
          const struct fs_data* filesys_data, 
          const void* mapped_file, 
          struct inode* ret_inode);

/*
 * Returns pointer to the block.
 */
void*
get_block_ptr(size_t block_index, 
              const struct fs_data* filesys_data,
              void* mapped_file);

/*
 * Gets block by index.
 */
void 
get_block(size_t block_index, 
          const struct fs_data* filesys_data, 
          const void* mapped_file, 
          void* ret_block);

/*
 * For file with inode index 'inode_index' returns ptr to offset in block
 */
void* 
get_ptr(size_t inode_index, 
        const struct fs_data* filesys_data,
        void* mapped_file,  
        ptrdiff_t offset);


#endif  // DATA_GETTERS_H