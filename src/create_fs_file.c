#include "create_fs_file.h"

//----------------------------------------------------------------------------//
size_t
count_fs_file_size(size_t size /*file size, KB*/)
{
    size_t blocks_cnt = ceil_div(size, BLOCK_SIZE);
    blocks_cnt = (blocks_cnt == 0) ? 1 : blocks_cnt;
    return INODES_SECTION_SIZE + 
           BLOCKS_INFO_SECTION_SIZE + blocks_cnt * BLOCK_SIZE;
}

//----------------------------------------------------------------------------//
int
create_fs_file(char* path, void** ret_ptr)
{
    if (ret_ptr == NULL)
    {   // Nowhere to return.
        return 1;
    }

    int fd = open(path, O_RDWR);

    if (fd == -1) 
    {   // Could not open file.
        return 2;
    }

    struct stat st;
    fstat(fd, &st);
    
    void* mmaped_ptr = 
        mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        
    if (mmaped_ptr == MAP_FAILED) 
    {   // Filed at mmap
        return 4;
    }
        
    *ret_ptr = mmaped_ptr;
    return 0;
}

//----------------------------------------------------------------------------//
int 
open_fs_file(char* path, size_t size, void** ret_ptr, bool rewrite)
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

    if (ret_ptr != NULL)
    {
        void* mmaped_ptr = 
            mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        
        if (mmaped_ptr == MAP_FAILED) 
        {   // Filed at mmap
            return 4;
        }
        
        *ret_ptr = mmaped_ptr;
    }

    close(fd);
    return 0;
}