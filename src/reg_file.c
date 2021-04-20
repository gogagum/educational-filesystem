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
    assert(curr_offset <= inode_ptr->size);
    void* inblock_ptr = get_ptr(inode_ptr, 
                                curr_offset,
                                filesys_data, 
                                mapped_file);
    size_t blk_idx = ceil_div(curr_offset, BYTES_BLOCK_SIZE);
    size_t to_the_end_of_block = blk_idx * BYTES_BLOCK_SIZE - curr_offset;
    size_t to_the_end_of_file = inode_ptr->size - curr_offset;
    size_t to_read = to_the_end_of_file;
    if (to_the_end_of_block < to_read)
    {
        to_read = to_the_end_of_block;
    }
    if (read_limit < to_the_end_of_block)
    {
        to_read = to_the_end_of_block;
    }
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
    assert(curr_offset < inode_ptr->size);
    void* inblock_ptr = get_ptr(inode_ptr, 
                                curr_offset, 
                                filesys_data, 
                                mapped_file);
    size_t blk_idx = ceil_div(curr_offset, BYTES_BLOCK_SIZE);
    size_t to_the_end_of_block = (blk_idx + 1) * BYTES_BLOCK_SIZE - curr_offset;
    size_t to_write = 
        (to_the_end_of_block < write_limit) ? to_the_end_of_block : write_limit;
    memcpy(inblock_ptr, buff, to_write);
    return to_write;
}

//----------------------------------------------------------------------------//
int
grow_file(size_t bytes_to_add,
          inode_idx_t inode_idx,
          struct fs_data* filesys_data,
          void* mapped_file)
{
    struct inode file_inode;

    get_inode(&file_inode, inode_idx, filesys_data, mapped_file);

    if (file_inode.size + bytes_to_add > FILE_MAX_BLOCKS_CNT * BLOCK_SIZE)
    {
        return -1;
    }

    size_t curr_blocks_cnt = file_inode.blocks_cnt;
    size_t blocks_to_add = 
        ceil_div(file_inode.size + bytes_to_add, BYTES_BLOCK_SIZE) -
        file_inode.blocks_cnt;

    size_t successfully_added_blocks_cnt = 0;

    for (size_t i = 0; i < blocks_to_add; ++i)
    {
        size_t allocated_blk_idx = idx_alloc_blk(filesys_data, mapped_file);

        size_t add = (allocated_blk_idx != filesys_data->blocks_cnt) ? 1 : 0;

        if (add)
        {
            ++successfully_added_blocks_cnt;
            file_inode.blocks[curr_blocks_cnt + i] =
            allocated_blk_idx;
        }
    }

    if (successfully_added_blocks_cnt < blocks_to_add)
    {
        for (size_t i = 0; i < successfully_added_blocks_cnt; ++i)
        {
            free_blk(file_inode.blocks[curr_blocks_cnt + i],
                     filesys_data, 
                     mapped_file);
        }
        return -1;
    }
    file_inode.size += bytes_to_add;
    file_inode.blocks_cnt += blocks_to_add;

    set_inode(inode_idx, filesys_data, mapped_file, &file_inode);
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
    inode_idx_t created_inode_idx = idx_alloc_inode(filesys_data, mapped_file);

    struct inode default_reg_inode =
    {
        .type = REG,  // File type flag
        .size = 0,          // Size of the file
        .blocks_cnt = 0
    };

    set_inode(created_inode_idx, filesys_data, mapped_file, &default_reg_inode);

    struct inode* parent_inode_ptr = 
        get_inode_ptr(parent_inode_idx, filesys_data, mapped_file);
    size_t curr_parent_dir_links_cnt = 
        get_dir_links_cnt(parent_inode_ptr, filesys_data, mapped_file);

    struct link in_parent_link = {.inode_idx = created_inode_idx};
    strcpy((void*)&in_parent_link.name, filename);
    grow_file(sizeof(struct link),
              parent_inode_idx,
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

