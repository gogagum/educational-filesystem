#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string.h>
#include <stddef.h>
// #include "fs.h"
#include "data_getters.h"

/*
 * Returns ith internal file/directory link pointer.
 */
struct link*
get_ith_internal_file_link_ptr(struct inode* dir_inode_ptr,
                               size_t inturnal_index,
                               const struct fs_data* filesys_data, 
                               const void* mapped_file);


/*
 * Returns ith internal file/directory link.
 */
void
get_ith_internal_file_link(struct link* ret_link,
                           struct inode* dir_inode_ptr,
                           size_t inturnal_index,
                           const struct fs_data* filesys_data, 
                           const void* mapped_file);

/*
 * Search for a file by name in dir. Returns NULL if file_not_found.
 */
struct inode*
find_inode_ptr_by_name(char* internal_file_name,
                       struct inode* dir_inode_ptr,
                       const struct fs_data* filesys_data, 
                       const void* mapped_file);


#endif