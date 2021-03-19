#include "directory.h"

//----------------------------------------------------------------------------//
struct link*
get_ith_internal_file_link_ptr(struct inode* dir_inode_ptr,
                               size_t inturnal_idx,
                               const struct fs_data* filesys_data, 
                               const void* mapped_file)
{
    assert((*dir_inode_ptr).type == DIR);
    assert(inturnal_idx * sizeof(struct link) < (*dir_inode_ptr).size);
    return get_ptr(dir_inode_ptr,
                   inturnal_idx * sizeof(struct link), 
                   filesys_data, 
                   (void*)mapped_file);
}


//----------------------------------------------------------------------------//
void
get_ith_internal_file_link(struct link* ret_link,
                           struct inode* dir_inode_ptr,
                           size_t inturnal_idx,
                           const struct fs_data* filesys_data, 
                           const void* mapped_file)
{
    memcpy(ret_link, 
           get_ith_internal_file_link_ptr(dir_inode_ptr, 
                                          inturnal_idx, 
                                          filesys_data, 
                                          mapped_file),
           sizeof(struct link));
}

//----------------------------------------------------------------------------//
struct inode*
find_inturnal_inode_ptr_by_name(char* internal_file_name,
                                struct inode* dir_inode_ptr,
                                const struct fs_data* filesys_data, 
                                const void* mapped_file)
{
    assert((*dir_inode_ptr).type == DIR);
    size_t links_cnt = (*dir_inode_ptr).size / sizeof(struct link);
    struct inode* ret_inode_ptr;
    for (size_t i = 0; i < links_cnt; i++)
    {
        struct link* link_ptr = get_ith_internal_file_link_ptr(dir_inode_ptr, 
                                                               i, 
                                                               filesys_data, 
                                                               mapped_file);
        if (strcmp(internal_file_name, (*link_ptr).name))
        {
            ret_inode_ptr = get_inode_ptr((*link_ptr).inode_idx, 
                                          filesys_data, 
                                          (void*)mapped_file);
            break; // File found
        }   
    }
    return ret_inode_ptr;
}

//----------------------------------------------------------------------------//
struct inode*
find_inode_ptr_by_name(char* path_tail,
                       struct inode* dir_inode_ptr,
                       const struct fs_data* filesys_data,
                       const void *mapped_file)
{
    char c = '/';
    char* end_of_path_step = strstr(path_tail, &c);

    int path_step_len;

    if (end_of_path_step == NULL)
    {
        path_step_len = strlen(path_tail);     
    }
    else
    {
        path_step_len = end_of_path_step - path_tail + 1;
    }

    char dir_name[path_step_len + 1];

    memcpy(dir_name, path_tail, path_step_len);
    dir_name[path_step_len] = 0;

    struct inode* next_step_inode_ptr = 
        find_inturnal_inode_ptr_by_name(dir_name, dir_inode_ptr, filesys_data, mapped_file);

    if (next_step_inode_ptr == NULL)
    {
        return NULL;
    }

    return find_inode_ptr_by_name(path_tail + path_step_len + 1, 
                                  next_step_inode_ptr, 
                                  filesys_data, 
                                  mapped_file);
}

//----------------------------------------------------------------------------//
size_t
find_inode_idx_by_name(char* path_tail,
                        struct inode* dir_inode_ptr,
                        const struct fs_data* filesys_data,
                        const void* mapped_file)
{
    char c = '/';
    char* end_of_path_step = strstr(path_tail, &c);

    int path_step_len;

    if (end_of_path_step == NULL)
    {   // last path step, no slash in the end
        path_step_len = strlen(path_tail);     
    }
    else
    {
        path_step_len = end_of_path_step - path_tail + 1;
    }

    char dir_name[path_step_len + 1];

    memcpy(dir_name, path_tail, path_step_len);
    dir_name[path_step_len] = 0;
} 

//----------------------------------------------------------------------------//
struct dir_data* 
get_dir_data_ptr(struct inode* dir_inode_ptr,
                 const struct fs_data* filesys_data,
                 void* mapped_file)
{
   return (struct dir_data*)get_ptr(dir_inode_ptr, 0, filesys_data, mapped_file); 
}
