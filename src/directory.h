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
                               size_t inturnal_idx,
                               const struct fs_data* filesys_data, 
                               const void* mapped_file);


/*
 * Returns ith internal file/directory link.
 */
void
get_ith_internal_file_link(struct link* ret_link,
                           struct inode* dir_inode_ptr,
                           size_t inturnal_idx,
                           const struct fs_data* filesys_data, 
                           const void* mapped_file);

/*
 * Search for a file by name in dir. Returns NULL if file not found.
 */
struct inode*
find_inturnal_inode_ptr_by_name(char* internal_file_name,
                                struct inode* dir_inode_ptr,
                                const struct fs_data* filesys_data, 
                                const void* mapped_file);

/*
 * Search for a file by name. Returns NULL if file is not found.
 * Returns NULL, if path is not found.
 */
struct inode*
find_inode_ptr_by_name(char* path_tail,
                       struct inode* dir_inode_ptr,
                       const struct fs_data* filesys_data,
                       const void* mapped_file);


/*
 * Search for a file by name. Returns NULL if file is not found.
 * Returns (*fs_data).inodes_cnt, if path is not found.
 */
size_t
find_inode_idx_by_name(char* path_tail,
                       struct inode* dir_inode_ptr,
                       const struct fs_data* filesys_data,
                       const void* mapped_file);

/*
 * Get pointer to directory info.
 */
struct dir_data* 
get_dir_data_ptr(struct inode* dir_inode_ptr,
                 const struct fs_data* filesys_data,
                 void* mapped_file);


#endif