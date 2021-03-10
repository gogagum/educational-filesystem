#include <assert.h>
#include "data_getters.h"

/*
 * Fills fs data struct from pointer
 */
void get_data(struct fs_data* filesys_data, void* mapped_file)
{
    memcpy((void*)filesys_data, mapped_file, sizeof(struct fs_data));
}

/*
 * Gets inode by index.
 */
void get_inode(size_t i, const struct fs_data* filesys_data, 
               void* mapped_file, struct inode* ret_inode)
{
    assert(i <= (*filesys_data).inodes_cnt);
    memcpy((void*)ret_inode, 
           mapped_file + BLOCKS_INFO_SECTION_SIZE * BLOCK_SIZE * 1024,
           sizeof(struct inode));
}

/*
 * Gets block by index.
 */
void get_block(size_t i, const struct fs_data* filesys_data, 
               void* mapped_file, void* ret_block)
{
    assert(i <= (*filesys_data).blocks_cnt);
    memcpy(ret_block, 
           mapped_file + BLOCKS_INFO_SECTION_SIZE * BLOCK_SIZE * 1024 + 
           (*filesys_data).inodes_cnt * sizeof(struct inode),
           BLOCK_SIZE * 1024);
}