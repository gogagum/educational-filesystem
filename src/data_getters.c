#include "data_getters.h"

/*
 * Fills fs data struct from pointer
 */
void 
get_filesys_data(struct fs_data* filesys_data, 
                 const void* mapped_file)
{
    memcpy((void*)filesys_data, mapped_file, sizeof(struct fs_data));
}

/*
 * Gets inode pointer by index.
 */
struct inode*
get_inode_ptr(size_t inode_index,
              const struct fs_data* filesys_data,
              void* mapped_file)
{
    assert(inode_index <= (*filesys_data).inodes_cnt);
    return mapped_file + BLOCKS_INFO_SECTION_SIZE * BLOCK_SIZE * 1024;
}

/*
 * Gets inode by index.
 */
void get_inode(size_t inode_index, 
               const struct fs_data* filesys_data, 
               const void* mapped_file, 
               struct inode* ret_inode)
{
    memcpy((void*)ret_inode, 
           get_inode_ptr(inode_index, filesys_data, (void*)mapped_file),
           sizeof(struct inode));
}

/*
 * Returns pointer to the block.
 */
void*
get_block_ptr(size_t block_index, 
              const struct fs_data* filesys_data,
              void* mapped_file)
{
    assert(block_index <= (*filesys_data).blocks_cnt);
    return mapped_file + BLOCKS_INFO_SECTION_SIZE * BLOCK_SIZE * 1024 + 
           (*filesys_data).inodes_cnt * sizeof(struct inode);
}


/*
 * Gets block by index.
 */
void 
get_block(size_t block_index, 
          const struct fs_data* filesys_data, 
          const void* mapped_file, 
          void* ret_block)
{
    memcpy(ret_block, 
           get_block_ptr(block_index, filesys_data, (void*)mapped_file),
           BLOCK_SIZE * 1024);
}

/*
 * For file with inode index 'inode_index' returns ptr to offset in block
 */
void* 
get_ptr(size_t inode_index, 
        const struct fs_data* filesys_data,
        void* mapped_file,  
        ptrdiff_t offset)
{
    struct inode file_inode;
    get_inode(inode_index, filesys_data, mapped_file, &file_inode);
    assert(offset >= file_inode.size);
    size_t infile_block_index = offset / (BLOCK_SIZE * 1024); 
    size_t inblock_offset = offset % (BLOCK_SIZE * 1024);
    size_t block_index = file_inode.blocks[infile_block_index];
    return get_block_ptr(block_index, filesys_data, mapped_file) + inblock_offset;
}

