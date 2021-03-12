#include "alloc.h"

//----------------------------------------------------------------------------//
void*
ptr_alloc_blk(const struct fs_data* filesys_data,
              void* mapped_file)
{
    size_t last_freed_block_idx = (*filesys_data).blocks_stack_beginning;
    if (last_freed_block_idx < (*filesys_data).blocks_cnt)
    {
        void* blk_ptr = get_block_ptr(last_freed_block_idx, 
                                      filesys_data, 
                                      mapped_file);
        size_t pre_last_freed_blk_idx = *blk_ptr;
        (*filesys_data).blocks_stack_beginning = pre_last_freed_blk_idx;
        memset(block_ptr, 0, BLOCK_SIZE * 1024);
        return block_ptr;
    }
    else
    {
        size_t blk_idx = (*filesys_data).blocks_tail_beginning;
        if (blk_idx < (*filesys_data).blocks_cnt)
        {
            return get_block_ptr(blk_idx, filesys_data, mapped_file);
        }
    }
    return NULL;
}

//----------------------------------------------------------------------------//
size_t
idx_alloc_blk(const struct fs_data* filesys_data,
              void* mapped_file)
{
    size_t last_freed_block_idx = (*filesys_data).blocks_stack_beginning;
    if (last_freed_block_idx < (*filesys_data).blocks_cnt)
    {
        void* blk_ptr = get_block_ptr(last_freed_block_idx, 
                                      filesys_data, 
                                      mapped_file);
        size_t pre_last_freed_blk_idx = *blk_ptr;
        (*filesys_data).blocks_stack_beginning = pre_last_freed_blk_idx;
        memset(block_ptr, 0, BLOCK_SIZE * 1024);
        return last_freed_block_idx;
    }
    else
    {
        size_t blk_idx = (*filesys_data).blocks_tail_beginning;
        if (blk_idx < (*filesys_data).blocks_cnt)
        {
            return blk_idx;
        }
    }
    return (*filesys_data).blocks_cnt;
}

//----------------------------------------------------------------------------//
void*
free_blk(size_t block_index,
         const struct fs_data* filesys_data,
         void* mapped_file)
{
    void* freed_block_ptr = get_block_ptr(block_index, 
                                          filesys_data, 
                                          mapped_file);
    memset(freed_block_ptr, 0, BLOCK_SIZE * 1024);
    *freed_block_ptr = (*filesys_data).blocks_stack_beginning;
    (*filesys_data).blocks_stack_beginning = block_index;
}