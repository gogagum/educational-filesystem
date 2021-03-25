#include "reg_file.h"

#define ceil_div(a, b) (a-1) / b + 1

//----------------------------------------------------------------------------//
size_t
get_chunk(void* buff,
          struct inode* inode_ptr,
          size_t read_limit,
          ptrdiff_t curr_offset,
          const struct fs_data* filesys_data,
          void* mapped_file)
{
    void* inblock_ptr = get_ptr(inode_ptr, 
                                curr_offset, 
                                filesys_data, 
                                mapped_file);
    size_t blk_idx = ceil_div(curr_offset, BYTES_BLOCK_SIZE);
    size_t to_the_end_of_block = blk_idx * BYTES_BLOCK_SIZE - curr_offset;
    size_t to_read = 
        (to_the_end_of_block < read_limit) ? to_the_end_of_block : read_limit;
    memcpy(buff, inblock_ptr, to_read);
    return to_read;
}

//----------------------------------------------------------------------------//
size_t
set_chunk(void* buff,
          struct inode* inode_ptr,
          size_t write_limit,
          ptrdiff_t curr_offset,
          const struct fs_data* filesys_data,
          void* mapped_file)
{
    void* inblock_ptr = get_ptr(inode_ptr, 
                                curr_offset, 
                                filesys_data, 
                                mapped_file);
    size_t blk_idx = ceil_div(curr_offset, BYTES_BLOCK_SIZE);
    size_t to_the_end_of_block = blk_idx * BYTES_BLOCK_SIZE - curr_offset;
    size_t to_write = 
        (to_the_end_of_block < write_limit) ? to_the_end_of_block : write_limit;
    memcpy(inblock_ptr, buff, to_write);
}

//----------------------------------------------------------------------------//
int
grow_file(size_t bytes_to_add,
          struct inode* inode_ptr,
          struct fs_data* filesys_data,
          void* mapped_file)
{
#ifdef DEBUG
    printf("grow_file(%li, %p, %p, %p)\n", 
           bytes_to_add, 
           inode_ptr, 
           filesys_data, 
           mapped_file);
#endif
    size_t curr_file_size = inode_ptr->size;
    size_t curr_blocks_cnt = inode_ptr->blocks_cnt;
    size_t blocks_to_add = 
        ceil_div(curr_file_size + bytes_to_add, BYTES_BLOCK_SIZE) - 
        curr_blocks_cnt;
#ifdef DEBUG
    printf("File size before growth: %li.\n", curr_file_size);
    printf("File blocks count: %li.\n", curr_blocks_cnt);
    printf("Number of blocks to alloc in grow_file: %li.\n", blocks_to_add);
#endif

    size_t successfully_added_blocks_cnt = 0;

    for (size_t i = 0; i < blocks_to_add; ++i)
    {
        size_t allocated_blk_idx = idx_alloc_blk(filesys_data, mapped_file);

        size_t add = (allocated_blk_idx != filesys_data->blocks_cnt) ? 1 : 0;

        if (add)
        {
            ++successfully_added_blocks_cnt;
            inode_ptr->blocks[curr_blocks_cnt + i] = 
            allocated_blk_idx; 
#ifdef DEBUG
            printf("Allocated block with index: %li.\n", inode_ptr->blocks[curr_blocks_cnt + i]);
#endif
        }
    }

    if (successfully_added_blocks_cnt < blocks_to_add)
    {
        for (size_t i = 0; i < successfully_added_blocks_cnt; ++i)
        {
            free_blk(inode_ptr->blocks[curr_blocks_cnt + i], 
                     filesys_data, 
                     mapped_file);
        }
        return -1;
    }
    inode_ptr->size += bytes_to_add;
    inode_ptr->blocks_cnt += blocks_to_add;
    return 0;

}

//----------------------------------------------------------------------------//
void
delete_file(size_t inode_idx, 
            struct fs_data* filesys_data,
            void *mapped_file)
{
    struct inode* inode_ptr = get_inode_ptr(inode_idx, 
                                            filesys_data, 
                                            mapped_file);
    assert(inode_ptr->type == REG);
    for (size_t i = 0; i < inode_ptr->blocks_cnt; ++i)
    {
        size_t block_idx = inode_ptr->blocks[i];
        free_blk(block_idx, filesys_data, mapped_file);
    }
    free_inode(inode_idx, filesys_data, mapped_file);
}

//----------------------------------------------------------------------------//
void
create_reg_inode(char* filename,
                 inode_idx_t parent_inode_idx,
                 struct fs_data* filesys_data,
                 void* mapped_file)
{
#ifdef DEBUG
    printf("create_inode(%s, %p, %p)\n",  
           filename, 
           filesys_data, 
           mapped_file);
#endif

    inode_idx_t created_inode_idx = idx_alloc_inode(filesys_data, mapped_file);
    struct inode* created_inode_ptr = 
        get_inode_ptr(created_inode_idx, filesys_data, mapped_file);

    struct inode default_reg_inode =
    {
        .type = REG,  // File type flag
        .size = 0,          // Size of the file
        .blocks_cnt = 0
    };

    set_inode(created_inode_idx, filesys_data, mapped_file, &default_reg_inode);

#ifdef DEBUG
    printf("Created inode index: %li.\n", created_inode_idx);
    printf("Created inode ptr: %p.\n", created_inode_ptr);
    printf("Created inode position: %li.\n", (void*)created_inode_ptr - mapped_file);
#endif

    struct inode* parent_inode_ptr = 
        get_inode_ptr(parent_inode_idx, filesys_data, mapped_file);
    size_t curr_parent_dir_links_cnt = 
        get_dir_links_cnt(parent_inode_ptr, filesys_data, mapped_file);

    struct link in_parent_link = {.inode_idx = created_inode_idx};
    strcpy((void*)&in_parent_link.name, filename);
    grow_file(sizeof(struct link), 
              parent_inode_ptr, 
              filesys_data, 
              mapped_file);
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

