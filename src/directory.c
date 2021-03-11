#include "directory.h"

//----------------------------------------------------------------------------//
struct link*
get_ith_internal_file_link_ptr(struct inode* dir_inode_ptr,
                               size_t inturnal_index,
                               const struct fs_data* filesys_data, 
                               const void* mapped_file)
{
    assert((*dir_inode_ptr).type == DIR);
    assert(inturnal_index * sizeof(struct link) < (*dir_inode_ptr).size);
    return get_ptr(dir_inode_ptr,
                   inturnal_index * sizeof(struct link), 
                   filesys_data, 
                   (void*)mapped_file);
}


//----------------------------------------------------------------------------//
void
get_ith_internal_file_link(struct link* ret_link,
                           struct inode* dir_inode_ptr,
                           size_t inturnal_index,
                           const struct fs_data* filesys_data, 
                           const void* mapped_file)
{
    memcpy(ret_link, 
           get_ith_internal_file_link_ptr(dir_inode_ptr, 
                                          inturnal_index, 
                                          filesys_data, 
                                          mapped_file),
           sizeof(struct link));
}

//----------------------------------------------------------------------------//
struct inode*
find_inode_ptr_by_name(char* internal_file_name,
                       struct inode* dir_inode_ptr,
                       const struct fs_data* filesys_data, 
                       const void* mapped_file)
{
    assert((*dir_inode_ptr).type == DIR);
    size_t links_cnt = (*dir_inode_ptr).size / sizeof(struct link);
    struct inode* ret_inode_ptr;
    for (size_t i = 0; i < links_cnt; i++)
    {
        struct link* link_ptr = get_ith_internal_file_link_ptr(dir_inode_ptr, i, filesys_data, mapped_file);
        if (strcmp(internal_file_name, (*link_ptr).name))
        {
            ret_inode_ptr = get_inode_ptr((*link_ptr).inode_index, filesys_data, (void*)mapped_file);
            break; // File found
        }   
    }
    return ret_inode_ptr;
}