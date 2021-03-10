#ifndef CREATE_FS_FILE_H
#define CREATE_FS_FILE_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>

#define INODES_SECTION_SIZE 4096   // 2 megabytes for inodes
#define BLOCKS_INFO_SECTION_SIZE 1 // 1 KB for free inodes, free blocks 
                                   // and file size info
#define BLOCK_SIZE 4               // block has a size of 4 KB
#define MAX_BLOCKS_CNT 65536       // max number of blocks

#define ceil_div(a, b) (a - 1) / b + 1

/*
 * Counts file size from number of kilobytes. 
 */
size_t
count_file_size(size_t size /*file size, KB*/);

/*
 * Creates file for file filesystem simulator. 
 * If ret_ptr != NULL, return pointer to beginning of mmapped file.
 */
int
create_fs_file(char* path, void** ret_ptr);

/*
 * Openes or creates file for file filesystem simulator. 
 * If ret_ptr != NULL, return pointer to beginning of mmapped file.
 */
int 
open_fs_file(char* path, size_t size, void** ret_ptr, bool rewrite);

#endif // CREATE_FS_FILE_H