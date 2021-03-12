#ifndef REG_FILE_H
#define REG_FILE_H

#include <stddef.h>
#include <string.h>
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

#endif  // REG_FILE_H