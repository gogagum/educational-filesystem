#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string.h>
// #include "fs.h"
#include "data_getters.h"

/*
 * Returns ith internal file/directory
 */
void
get_ith_internal_file(size_t inode_index,
                      const struct fs_data* filesys_data, 
                      const void* mapped_file,
                      size_t inturnal_index,
                      struct link* ret_link);





#endif