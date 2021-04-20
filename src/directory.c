#include "directory.h"

const char* UP_DIR = "..\0";
const char* CURR_DIR = ".\0";

//----------------------------------------------------------------------------//
struct link*
get_ith_internal_file_link_ptr(struct inode* dir_inode_ptr,
                               size_t internal_idx,
                               const struct fs_data* filesys_data, 
                               const void* mapped_file)
{
    assert(dir_inode_ptr->type == DIR);
    assert((internal_idx + 1) * sizeof(struct link) <
           dir_inode_ptr->size);
    return get_ptr(dir_inode_ptr,
                   (ptrdiff_t)((internal_idx + 1) * sizeof(struct link)),
                   filesys_data, 
                   (void*)mapped_file);
}


//----------------------------------------------------------------------------//
void
get_ith_internal_file_link(struct link* ret_link,
                           struct inode* dir_inode_ptr,
                           size_t internal_idx,
                           const struct fs_data* filesys_data, 
                           const void* mapped_file)
{
    memcpy(ret_link, 
           get_ith_internal_file_link_ptr(dir_inode_ptr, 
                                          internal_idx,
                                          filesys_data, 
                                          mapped_file),
           sizeof(struct link));
}

//----------------------------------------------------------------------------//
struct inode*
find_internal_inode_ptr_by_name(char* internal_file_name,
                                struct inode* dir_inode_ptr,
                                const struct fs_data* filesys_data,
                                const void* mapped_file)
{
    if (dir_inode_ptr->type != DIR)
    {
        return NULL;
    }

    size_t links_cnt = 
        get_dir_links_cnt(dir_inode_ptr, filesys_data, mapped_file);
    struct inode* ret_inode_ptr = NULL;
    for (size_t i = 0; i < links_cnt; ++i)
    {
        struct link* link_ptr = get_ith_internal_file_link_ptr(dir_inode_ptr, 
                                                               i, 
                                                               filesys_data, 
                                                               mapped_file);
        if (!strcmp(internal_file_name, link_ptr->name))
        {
            ret_inode_ptr = get_inode_ptr(link_ptr->inode_idx, 
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
    inode_idx_t inode_idx = find_inode_idx_by_name(path_tail, 
                                                   dir_inode_ptr, 
                                                   filesys_data, 
                                                   mapped_file);
    return (inode_idx == filesys_data->inodes_cnt) ? 
           NULL : 
           get_inode_ptr(inode_idx, filesys_data, (void*)mapped_file);
}

//----------------------------------------------------------------------------//
size_t
find_inode_idx_by_name(const char* path_tail,
                       struct inode* dir_inode_ptr,
                       const struct fs_data* filesys_data,
                       const void* mapped_file)
{
    path_tail += 1;

    if (path_tail[0] == '\0')
    {   // No need to move. Path finishes with '/'        
        return get_inode_idx_by_ptr(dir_inode_ptr, filesys_data, mapped_file);
    }

    char c[] = "/\0";
    char* end_of_path_step = strstr(path_tail, c);

    ptrdiff_t path_step_len;

    if (end_of_path_step == NULL)
    {
      path_step_len = (ptrdiff_t)strlen(path_tail);
    }
    else
    {
      path_step_len = end_of_path_step - path_tail + 1;
    }

    char dir_name[path_step_len + 1];

    memcpy(dir_name, path_tail, path_step_len);
    dir_name[path_step_len] = '\0';

    struct inode* next_step_inode_ptr =
        find_internal_inode_ptr_by_name(dir_name,
                                        dir_inode_ptr,
                                        filesys_data,
                                        mapped_file);

    if (next_step_inode_ptr == NULL)
    {   // Failed.
        return filesys_data->inodes_cnt;
    }

    return find_inode_idx_by_name(path_tail + path_step_len + 1, 
                                  next_step_inode_ptr, 
                                  filesys_data, 
                                  mapped_file);
} 

//----------------------------------------------------------------------------//
struct link*
find_link_ptr_by_inode_idx(size_t inode_idx,
                           struct inode* dir_inode_ptr,
                           const struct fs_data* filesys_data,
                           const void* mapped_file)
{
    size_t links_cnt = get_dir_links_cnt(dir_inode_ptr, 
                                         filesys_data, 
                                         mapped_file);
    struct link* ret_link_ptr = NULL;

    for (size_t i = 0; i < links_cnt; i++)
    {
        ret_link_ptr = get_ith_internal_file_link_ptr(dir_inode_ptr, 
                                                      i, 
                                                      filesys_data, 
                                                      mapped_file);
        if (ret_link_ptr->inode_idx == inode_idx)
        {
            return ret_link_ptr;
        }
    }
    return NULL;
}

//----------------------------------------------------------------------------//
size_t  
get_dir_links_cnt(struct inode* dir_inode_ptr,
                  const struct fs_data* filesys_data,
                  const void* mapped_file)
{
   return *(size_t*)get_ptr(dir_inode_ptr, 0, filesys_data, (void*)mapped_file); 
}

//----------------------------------------------------------------------------//
void
set_dir_links_cnt(size_t links_cnt,
                  struct inode* dir_inode_ptr,
                  const struct fs_data* filesys_data,
                  void* mapped_file)
{
    size_t* cnt_ptr = get_ptr(dir_inode_ptr, 0, filesys_data, (void*)mapped_file);
    *cnt_ptr = links_cnt;
}

//----------------------------------------------------------------------------//
int
create_dir_inode(inode_idx_t parent_inode_idx,
                 char* filename,
                 struct fs_data* filesys_data,
                 void* mapped_file)
{
    inode_idx_t created_inode_idx = idx_alloc_inode(filesys_data, mapped_file);

    struct inode default_dir_inode =
    {
        .type = DIR,        // File type flag
        .size = 0,          // Size of the file
        .blocks_cnt = 0
    };

    struct inode* created_inode_ptr = 
        get_inode_ptr(created_inode_idx, filesys_data, mapped_file);
 
    set_inode(created_inode_idx, filesys_data, mapped_file, &default_dir_inode);

    if (created_inode_idx != 0) 
    {
        struct inode* parent_inode_ptr = 
            get_inode_ptr(parent_inode_idx, filesys_data, mapped_file);
        size_t curr_parent_dir_links_cnt = 
            get_dir_links_cnt(parent_inode_ptr, filesys_data, mapped_file);

        struct link in_parent_link = {.inode_idx = created_inode_idx};
        strcpy((void*)&in_parent_link.name, filename);
        int grow_result = grow_file(sizeof(struct link),
                                    parent_inode_ptr,
                                    filesys_data,
                                    mapped_file);
        if (grow_result == -1)
        {
            return -1;
        }
        set_link(curr_parent_dir_links_cnt, 
                 &in_parent_link, 
                 parent_inode_ptr, 
                 filesys_data, 
                 mapped_file);
        set_dir_links_cnt(curr_parent_dir_links_cnt + 1, 
                          parent_inode_ptr, 
                          filesys_data, 
                          mapped_file);
    }

    int grow_result = grow_file(3 * sizeof(struct link),
                                // 2 links and 1 link cell is for size_t links count
                                created_inode_ptr,
                                filesys_data,
                                mapped_file);

    if (grow_result == -1)
    {
        return -1;
    }

    struct link loop_link = {.inode_idx = created_inode_idx, .name = ".\0"};
    struct link parent_link = {.inode_idx = parent_inode_idx, .name = "..\0"};
    
    set_dir_links_cnt(2, created_inode_ptr, filesys_data, mapped_file);

    set_link(0, &parent_link, created_inode_ptr, filesys_data, mapped_file);
    set_link(1, &loop_link, created_inode_ptr, filesys_data, mapped_file);

    return 0;
}

//----------------------------------------------------------------------------//
void
remove_inode(struct inode* dir_inode_ptr,
             struct fs_data* filesys_data,
             void* mapped_file)
{
    if (dir_inode_ptr->type == DIR)
    {
        size_t links_cnt = get_dir_links_cnt(dir_inode_ptr, 
                                             filesys_data, 
                                             mapped_file);
        for (size_t i = 0; i < links_cnt; ++i)
        {
            struct link* curr_link = 
                get_ith_internal_file_link_ptr(dir_inode_ptr, 
                                               i, 
                                               filesys_data, 
                                               mapped_file);
            if (!strcmp(curr_link->name, UP_DIR) && 
                !strcmp(curr_link->name, CURR_DIR))
            {
                struct inode* internal_file_inode_ptr = 
                    get_inode_ptr(curr_link->inode_idx, 
                                  filesys_data, 
                                  mapped_file);
                remove_inode(internal_file_inode_ptr, filesys_data, mapped_file);
            }
        }
    }
    for (size_t i = 0; i < dir_inode_ptr->blocks_cnt; ++i)
    {
        free_blk(dir_inode_ptr->blocks[i], filesys_data, mapped_file);
    }
}

//----------------------------------------------------------------------------//
size_t
get_parent_directory_inode_idx(struct inode* dir_inode_ptr,
                               struct fs_data* filesys_data,
                               void* mapped_file)
{
    return find_inode_idx_by_name(UP_DIR,
                                  dir_inode_ptr, 
                                  filesys_data, 
                                  mapped_file);
}

//----------------------------------------------------------------------------//
struct inode*
get_parent_directory_inode_ptr(struct inode* dir_inode_ptr, 
                               struct fs_data* filesys_data,
                               void* mapped_file)
{
    inode_idx_t parent_directory_inode_idx =
        get_parent_directory_inode_idx(dir_inode_ptr, 
                                       filesys_data, 
                                       mapped_file);
    return get_inode_ptr(parent_directory_inode_idx, filesys_data, mapped_file);
}

//----------------------------------------------------------------------------//
void
set_link(size_t link_idx,
         struct link* link,
         struct inode* dir_inode_ptr,
         struct fs_data* filesys_data,
         void* mapped_file)
{
    struct link* link_ptr = get_ith_internal_file_link_ptr(dir_inode_ptr, 
                                                           link_idx, 
                                                           filesys_data, 
                                                           mapped_file);
    memcpy(link_ptr, link, sizeof(struct link));
}
