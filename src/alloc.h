#ifndef ALLOC_H
#define ALLOC_H

#include "data_getters.h"
//#include "fs.h"

/*
 * Returns pointer to a new allocated block
 */
void*
ptr_alloc_blk(struct fs_data* filesys_data,
              void* mapped_file);

/*
 * Returns index of a new allocated block
 */ 
size_t
idx_alloc_blk(struct fs_data* filesys_data,
              void* mapped_file);

/*
 * Free block by index
 */
void
free_blk(size_t block_idx,
         struct fs_data* filesys_data,
         void* mapped_file);

/*
 * Free inode of a deleted file.
 */
void
free_inode(size_t inode_idx, 
           struct fs_data* filesys_data,
           void* mapped_file);



#endif  // ALLOC_H
