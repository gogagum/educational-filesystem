#include "alloc.h"

//----------------------------------------------------------------------------//
size_t
idx_alloc_blk(struct fs_data* filesys_data,
              void* mapped_file)
{
    size_t last_freed_block_idx = filesys_data->blocks_stack_beginning;
    if (last_freed_block_idx < filesys_data->blocks_cnt)
    {
        void* block_ptr = get_block_ptr(last_freed_block_idx, 
                                        filesys_data, 
                                        mapped_file);
        size_t pre_last_freed_blk_idx = *(size_t*)block_ptr;
        filesys_data->blocks_stack_beginning = pre_last_freed_blk_idx;
        memset(block_ptr, 0, BYTES_BLOCK_SIZE);
        return last_freed_block_idx;
    }
    else
    {
        size_t blk_idx = filesys_data->blocks_tail_beginning;
        if (blk_idx < filesys_data->blocks_cnt)
        {
            return blk_idx;
        }
    }
    
    return filesys_data->blocks_cnt;
}

//----------------------------------------------------------------------------//
inode_idx_t
idx_alloc_inode(struct fs_data* filesys_data,
                void* mapped_file)
{
    inode_idx_t last_freed_inode_idx = filesys_data->inodes_stack_beginning;
    if (last_freed_inode_idx < filesys_data->inodes_cnt)
    {
        struct inode* inode_ptr = get_inode_ptr(last_freed_inode_idx, 
                                                filesys_data, 
                                                mapped_file);
        inode_idx_t pre_last_freed_inode_idx = 
            *(inode_idx_t*)pre_last_freed_inode_idx;
        filesys_data->inodes_stack_beginning = pre_last_freed_inode_idx;
        memset(inode_ptr, 0, sizeof(struct inode));
        return last_freed_inode_idx;
    }
    else
    {
        inode_idx_t inode_idx = filesys_data->inodes_tail_beginning;
        if (inode_idx < filesys_data->inodes_cnt)
        {
            return inode_idx;
        }
    }
}

//----------------------------------------------------------------------------//
void
free_blk(size_t block_idx,
         struct fs_data* filesys_data,
         void* mapped_file)
{
    void* freed_block_ptr = get_block_ptr(block_idx, 
                                          filesys_data, 
                                          mapped_file);
    memset(freed_block_ptr, 0, BYTES_BLOCK_SIZE);
    *(size_t*)freed_block_ptr = filesys_data->blocks_stack_beginning;
    filesys_data->blocks_stack_beginning = block_idx;
}

//----------------------------------------------------------------------------//
void
free_inode(size_t inode_idx,
           struct fs_data* filesys_data,
           void* mapped_file)
{
    struct inode* freed_inode_ptr = get_inode_ptr(inode_idx, 
                                                  filesys_data, 
                                                  mapped_file);
    memset(freed_inode_ptr, 0, sizeof(struct inode));
    *(size_t *)freed_inode_ptr = filesys_data->inodes_stack_beginning;
    filesys_data->inodes_stack_beginning = inode_idx;
}
