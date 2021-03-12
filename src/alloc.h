#ifndef ALLOC_H
#define ALLOC_H

#include "data_getters.h"
//#include "fs.h"

/*
 * Returns pointer to a new allocated block
 */
void*
ptr_alloc_blk(const struct fs_data* filesys_data,
              void* mapped_file);

/*
 * Returns index of a new allocated block
 */ 
size_t
idx_alloc_blk(const struct fs_data* filesys_data,
              void* mapped_file);

/*
 * Free block by index
 */
void*
free_blk(size_t block_index,
         const struct fs_data* filesys_data,
         void* mapped_file);





#endif  // ALLOC_H