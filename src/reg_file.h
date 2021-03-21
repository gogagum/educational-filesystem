#ifndef REG_FILE_H
#define REG_FILE_H

#include <stddef.h>
#include <string.h>
#include "fs.h"
#include "alloc.h"
#include "data_getters.h"

/*
 * Writes up to read_limit to buffer from file
 * until end of file. 
 */
size_t
get_chunk(void* buff,
          struct inode* inode_ptr,
          size_t read_limit,
          ptrdiff_t curr_offset,
          const struct fs_data* filesys_data,
          void* mapped_file);

/*
 * Write into file from buffer.
 */
size_t
set_chunk(void* buff,
          struct inode* inode_ptr,
          size_t write_limit,
          ptrdiff_t curr_offset,
          const struct fs_data* filesys_dat,
          void* mapped_file);

/*
 * Make file bigger. 'inode_ptr' mast be a pointer to an inode in mapped file. 
 */
void
grow_file(size_t bytes_to_add,
          struct inode* inode_ptr,
          struct fs_data* filesys_data,
          void* mapped_file);

/*
 * Deletes file by inode index.
 */
void 
delete_file(size_t inode_idx,
            struct fs_data* filesys_data,
            void* mapped_file);

#endif  // REG_FILE_H
