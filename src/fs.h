#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <time.h>

/*
 * Filesystem info structure. One is expected to be written in the first block of file.
 */
struct fs_data
{
    uint16_t blocks_cnt;              // Index of first block in the file
    uint16_t blocks_tail_beginning;   // Index of first block position which is free and after which all nodes are free.
    uint16_t blocks_stack_beginning;  // Index of first freed block position (that contains a stack info).
    uint16_t inodes_tail_beginning;   // Index of first inode position which is free and after which all nodes are free.
    uint16_t inodes_stack_beginning;  // Index of first freed inode position (that contains a stack info).
};

/*
 * Simplyfied inode structure.
 */
struct __attribute__((__packed__)) inode
{
    uint16_t blocks[27];  // Array of addresses
    uint8_t type;         // File type flag
    time_t last_edit;     // Moment of last edidtions
    uint8_t reserved;
};

#endif  // FS_H