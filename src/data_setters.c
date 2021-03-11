#include "data_setters.h"

/*
 * Copies filesys_data to the beginning of the file.
 */
void 
set_filesys_data(const struct fs_data* filesys_data, void* mapped_file)
{
    memcpy(mapped_file, (void*) filesys_data, sizeof(struct fs_data));
}

/*
 * Sets inode by index.
 */
void 
set_inode(size_t i, const struct fs_data* filesys_data, void* mapped_file, 
          const struct inode* inode_to_set)
{
    assert(i <= (*filesys_data).inodes_cnt);
    memcpy(mapped_file + BLOCKS_INFO_SECTION_SIZE * BLOCK_SIZE * 1024, 
           inode_to_set, sizeof(struct inode));
}

/*
 * Sets block by index.
 */
void 
set_block(size_t i, const struct fs_data* filesys_data, void* mapped_file, 
          const void* block_to_set)
{
    assert(i <= (*filesys_data).blocks_cnt);
    memcpy(mapped_file + BLOCKS_INFO_SECTION_SIZE * BLOCK_SIZE * 1024 + 
           (*filesys_data).inodes_cnt * sizeof(struct inode), 
           block_to_set, BLOCK_SIZE * 1024);
}