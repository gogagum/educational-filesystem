#ifndef DATA_GETTERS_H
#define DATA_GETTERS_H

#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "fs.h"

#ifdef DEBUG
#include <stdio.h>
#endif

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
get_inode_ptr(inode_idx_t inode_idx,
              const struct fs_data* filesys_data,
              void* mapped_file);

/*
 * Gets inode by index.
 */
void 
get_inode(struct inode* ret_inode,
          size_t inode_idx, 
          const struct fs_data* filesys_data, 
          const void* mapped_file);

/*
 * Returns pointer to the block.
 */
void*
get_block_ptr(size_t block_idx, 
              const struct fs_data* filesys_data,
              void* mapped_file);

/*
 * Gets block by index.
 */
void 
get_block(void* ret_block,
          size_t block_idx, 
          const struct fs_data* filesys_data, 
          const void* mapped_file);

/*
 * For file with inode reffered by 'inode_ptr' returns ptr to offset in block
 */
void*
get_ptr(const struct inode* inode_ptr,
        ptrdiff_t offset, 
        const struct fs_data* filesys_data,
        void* mapped_file);


/*
 * Returns root inode pointer
 */
struct inode*
get_root_inode_ptr(const struct fs_data* filesys_data,
                   void* mapped_file);


/*
 * Returns inode index by pointer.
 */
size_t get_inode_idx_by_ptr(struct inode* inode_ptr,
                            const struct fs_data* filesys_data,
                            const void* mapped_file);

#endif  // DATA_GETTERS_H
