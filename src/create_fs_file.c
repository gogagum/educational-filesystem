#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>

#define INODES_SECTION_SIZE 4096  // 2 megabytes for inodes
#define BLOCKS_INFO_SECTION_SIZE 1 // 1 KB for free inodes, free blocks, file size info
#define BLOCK_SIZE 4  // block has a size of 4 KB
#define MAX_BLOCKS_CNT 65536  // max number of blocks

#define ceil_div(a, b) (a - 1) / b + 1


/*
 * Counts file size from number of kilobytes. 
 */
size_t
count_file_size(size_t size /*file size, KB*/)
{
    size_t blocks_cnt = ceil_div(size, BLOCK_SIZE);
    blocks_cnt = (blocks_cnt == 0) ? 1 : blocks_cnt;
    return INODES_SECTION_SIZE + BLOCKS_INFO_SECTION_SIZE + blocks_cnt * BLOCK_SIZE;
}

/*
 * Creates file for file filesystem simulator. 
 * If ret_ptr != NULL, return pointer to beginning of mmapped file.
 */
int 
create_fs_file(char* path, size_t size, void** ret_ptr, bool rewrite)
{
    if (size > MAX_BLOCKS_CNT * BLOCK_SIZE) 
    {   // size is too large
        return 1;
    }
    
    if (access(path, F_OK) && !rewrite) 
    {   // File exists and don want to rewrite
        return 2;
    }

    int fd = open(path, O_RDWR | O_CREAT);

    if (fd == -1) 
    {   // Failed while opening
        return 3;
    }

    if (ret_ptr == NULL)
    {   // No need to return mmaped file
        return 0;
    }

    void* mmaped_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (mmaped_ptr == MAP_FAILED) 
    {   // Filed at mmap
        return 4;
    }

    *ret_ptr = mmaped_ptr;
    return 0;
}