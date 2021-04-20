#include "data_setters.h"

//----------------------------------------------------------------------------//
void 
set_filesys_data(const struct fs_data* filesys_data, 
                 void* mapped_file)
{
    memcpy(mapped_file, (void*)filesys_data, sizeof(struct fs_data));
}

//----------------------------------------------------------------------------//
void 
set_inode(size_t inode_idx, 
          const struct fs_data* filesys_data, 
          void* mapped_file, 
          const struct inode* inode_to_set)
{
    assert(inode_idx < filesys_data->inodes_cnt);
    memcpy(get_inode_ptr(inode_idx, filesys_data, mapped_file), 
           inode_to_set, 
           sizeof(struct inode));
}

//----------------------------------------------------------------------------//
void 
set_block(size_t block_idx, 
          const struct fs_data* filesys_data, 
          void* mapped_file, 
          const void* block_to_set)
{
    assert(block_idx < filesys_data->blocks_cnt);
    memcpy(get_block_ptr(block_idx, filesys_data, mapped_file), 
           block_to_set, 
           BYTES_BLOCK_SIZE);
}
