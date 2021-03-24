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
void
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
    for (size_t i = 0; i < blocks_to_add; ++i)
    {
        inode_ptr->blocks[curr_blocks_cnt + i] = 
        idx_alloc_blk(filesys_data, mapped_file);
#ifdef DEBUG
        printf("Added block %li to %p,\n", inode_ptr->blocks[curr_blocks_cnt + i], inode_ptr);
#endif
    }
    inode_ptr->size += bytes_to_add;
    inode_ptr->blocks_cnt += blocks_to_add;
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
