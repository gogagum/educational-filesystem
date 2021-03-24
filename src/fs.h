#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <time.h>

#define INODES_SECTION_SIZE 4096   // 2 megabytes for inodes
#define BLOCKS_INFO_SECTION_SIZE 1 // 1 KB for free inodes, free blocks 
                                   // and file size info
#define BLOCK_SIZE 4               // block has a size of 4 KB
#define BYTES_BLOCK_SIZE 4096      // block size in bytes
#define MAX_BLOCKS_CNT 65536       // max number of blocks

typedef size_t inode_idx_t;

enum FILE_TYPE
{
    DIR,
    REG
};

/*
 * Filesystem info structure. One is expected to be written in the first block of file.
 */
struct fs_data
{
    size_t blocks_cnt;              // Number of blocks in file
    size_t inodes_cnt;              // Number of inodes in file
    size_t blocks_tail_beginning;   // Index of first block position which is free and after which all nodes are free.
    size_t blocks_stack_beginning;  // Index of first freed block position (that contains a stack info).
    size_t inodes_tail_beginning;   // Index of first inode position which is free and after which all nodes are free.
    size_t inodes_stack_beginning;  // Index of first freed inode position (that contains a stack info).
};

/*
 * Simplyfied inode structure.
 */
struct __attribute__((__packed__)) inode
{
    size_t blocks[12];  // Array of addresses
    enum FILE_TYPE type;  // File type flag
    time_t last_edit;     // Moment of last edidtions
    size_t size;          // Size of the file
    size_t blocks_cnt;    // Number of blocks
    int32_t reserved;     // Void
};

/*
 * Info about one directory (which is written inside other directory).
 */
struct link
{
    size_t inode_idx;  // Index of an internal file inode
    char name[24];     // Name of an internal file
};

#endif  // FS_H
