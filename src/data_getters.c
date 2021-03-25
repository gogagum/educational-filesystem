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
get_inode_ptr(size_t inode_idx,
              const struct fs_data* filesys_data,
              void* mapped_file)
{
    assert(inode_idx < filesys_data->inodes_cnt);
    return mapped_file + BLOCKS_INFO_SECTION_SIZE * BYTES_BLOCK_SIZE +
                       + inode_idx * sizeof(struct inode);
}

//----------------------------------------------------------------------------//
void 
get_inode(struct inode* ret_inode,
          size_t inode_idx, 
          const struct fs_data* filesys_data, 
          const void* mapped_file)
{
    memcpy((void*)ret_inode, 
           get_inode_ptr(inode_idx, filesys_data, (void*)mapped_file),
           sizeof(struct inode));
}

//----------------------------------------------------------------------------//
void*
get_block_ptr(size_t block_idx, 
              const struct fs_data* filesys_data,
              void* mapped_file)
{
#ifdef DEBUG
    printf("block_idx: %li.\n", block_idx);
    printf("filesys_data->blocks_cnt: %li.\n", filesys_data->blocks_cnt);
#endif
    assert(block_idx < filesys_data->blocks_cnt);

    return mapped_file + BLOCKS_INFO_SECTION_SIZE * BYTES_BLOCK_SIZE + 
           filesys_data->inodes_cnt * sizeof(struct inode) + 
           BYTES_BLOCK_SIZE * block_idx;
}


//----------------------------------------------------------------------------//
void 
get_block(void* ret_block,
          size_t block_idx, 
          const struct fs_data* filesys_data, 
          const void* mapped_file)
{
    memcpy(ret_block, 
           get_block_ptr(block_idx, filesys_data, (void*)mapped_file),
           BYTES_BLOCK_SIZE);
}

//----------------------------------------------------------------------------//
void*
get_ptr(const struct inode* inode_ptr,  
        ptrdiff_t offset,
        const struct fs_data* filesys_data,
        void* mapped_file)
{
    assert(offset < inode_ptr->size);
    size_t inturnal_blk_idx = offset / BYTES_BLOCK_SIZE;
    assert(inturnal_blk_idx < 12);
    size_t inblock_offset = offset % BYTES_BLOCK_SIZE;
    return get_block_ptr(inode_ptr->blocks[inturnal_blk_idx], 
                         filesys_data, 
                         mapped_file) + 
           inblock_offset;
}

//----------------------------------------------------------------------------//
struct inode*
get_root_inode_ptr(const struct fs_data* filesys_data,
                   void* mapped_file)
{
    return get_inode_ptr(0, filesys_data, mapped_file);
}

//----------------------------------------------------------------------------//
size_t get_inode_idx_by_ptr(struct inode* inode_ptr,
                            const struct fs_data* filesys_data,
                            const void* mapped_file)
{
    struct inode* first_inode_ptr = 
        get_inode_ptr(0, filesys_data, (void*)mapped_file);
    assert(((void*)inode_ptr - (void*)first_inode_ptr) % 
           sizeof(struct inode) == 0);
    return (((void*)inode_ptr - (void*)first_inode_ptr) / sizeof(struct inode));
}
