#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string.h>
#include <stddef.h>
// #include "fs.h"
#include "data_getters.h"
#include "alloc.h"

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
find_inode_idx_by_name(const char* path_tail,
                       struct inode* dir_inode_ptr,
                       const struct fs_data* filesys_data,
                       const void* mapped_file);

/*
 * Finds link in directory links list by index.
 */
struct link*
find_link_ptr_by_inode_idx(size_t inode_idx,
                           struct inode* dir_inode_ptr,
                           const struct fs_data* filesys_data,
                           const void* mapped_file);

/*
 * Get number of links in directory.
 */
size_t 
get_dir_links_cnt(struct inode* dir_inode_ptr,
                  const struct fs_data* filesys_data,
                  const void* mapped_file);

/*
 * Set number of links in directory.
 */
void
set_dir_links_cnt(size_t links_cnt,
                  struct inode* dir_inode_ptr,
                  const struct fs_data* filesys_data,
                  const void* mapped_file);

/*
 *  Creates directory inode with standart links.
 *  Giving parent_inode_idx == 0 will work for root inode creation.
 */
void
create_dir_inode(inode_idx_t parent_inode_idx,
                 char* filename,
                 struct fs_data* filesys_data,
                 void* mapped_file);

/*
 * Removes directory and all internal directories.
 */
void
remove_inode(struct inode* dir_inode_ptr,
             struct fs_data* filesys_data,
             void* mapped_file);

/*
 * Finds parent inode index by finding link with name '..\n'.
 */
size_t
get_parent_directory_inode_idx(struct inode* dir_inode_ptr,
                               struct fs_data* filesys_data,
                               void* mapped_file);

/*
 * Finds parent inode pointer by finding link with name '..\n'.
 */
struct inode*
get_parent_directory_inode_ptr(struct inode* dir_inode_ptr, 
                               struct fs_data* filesys_data,
                               void* mapped_file);

/*
 * Sets link to index.
 */
void
set_link(size_t link_idx,
         struct link* link,
         struct inode* dir_inode_ptr,
         struct fs_data* filesys_data,
         void* mapped_file);

#endif
