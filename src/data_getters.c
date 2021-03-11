#include "data_getters.h"

//----------------------------------------------------------------------------//
void 
get_filesys_data(struct fs_data* filesys_data, 
                 const void* mapped_file)
{
    memcpy((void*)filesys_data, mapped_file, sizeof(struct fs_data));
}

//----------------------------------------------------------------------------//
struct inode*
get_inode_ptr(size_t inode_index,
              const struct fs_data* filesys_data,
              void* mapped_file)
{
    assert(inode_index <= (*filesys_data).inodes_cnt);
    return mapped_file + BLOCKS_INFO_SECTION_SIZE * BLOCK_SIZE * 1024;
}

//----------------------------------------------------------------------------//
void get_inode(struct inode* ret_inode,
               size_t inode_index, 
               const struct fs_data* filesys_data, 
               const void* mapped_file)
{
    memcpy((void*)ret_inode, 
           get_inode_ptr(inode_index, filesys_data, (void*)mapped_file),
           sizeof(struct inode));
}

//----------------------------------------------------------------------------//
void*
get_block_ptr(size_t block_index, 
              const struct fs_data* filesys_data,
              void* mapped_file)
{
    assert(block_index <= (*filesys_data).blocks_cnt);
    return mapped_file + BLOCKS_INFO_SECTION_SIZE * BLOCK_SIZE * 1024 + 
           (*filesys_data).inodes_cnt * sizeof(struct inode);
}


//----------------------------------------------------------------------------//
void 
get_block(void* ret_block,
          size_t block_index, 
          const struct fs_data* filesys_data, 
          const void* mapped_file)
{
    memcpy(ret_block, 
           get_block_ptr(block_index, filesys_data, (void*)mapped_file),
           BLOCK_SIZE * 1024);
}

//----------------------------------------------------------------------------//
void*
get_ptr(const struct inode* inode_ptr,  
        ptrdiff_t offset,
        const struct fs_data* filesys_data,
        void* mapped_file)
{
    assert(offset >= (*inode_ptr).size);
    size_t inturnal_block_index = offset / (BLOCK_SIZE * 1024);
    size_t inblock_offset = offset % (BLOCK_SIZE * 1024);
    return get_block_ptr(inturnal_block_index, filesys_data, mapped_file) + 
           inblock_offset;
}