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
    size_t blk_idx = ceil_div(curr_offset, BLOCK_SIZE * 1024);
    size_t to_the_end_of_block = blk_idx * BLOCK_SIZE * 1024 - curr_offset;
    size_t to_read = 
        (to_the_end_of_block < read_limit) ? to_the_end_of_block : read_limit;
    memcpy(buff, inblock_ptr, to_read);
    return to_read;
}