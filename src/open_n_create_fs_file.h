#ifndef CREATE_FS_FILE_H
#define CREATE_FS_FILE_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <error.h>
#include <stdio.h>  // TODO erase when finished with debug
#include "fs.h"
#include "data_setters.h"

#define ceil_div(a, b) (a - 1) / b + 1

/*
 * Counts file size from number of kilobytes. 
 */
size_t
count_fs_file_size(size_t size /*file size, KB*/);

/*
 * Creates file for file filesystem simulator. 
 * If ret_ptr != NULL, return pointer to beginning of mmapped file.
 */
void*
create_fs_file(int* ret_fd,
               char* path,
               size_t inodes_cnt,
               size_t blocks_cnt);

/*
 * Openes or creates file for file filesystem simulator. 
 * If ret_ptr != NULL, return pointer to beginning of mmapped file.
 */
void* 
open_fs_file(int* ret_fd, 
             char* path);

#endif // CREATE_FS_FILE_H
