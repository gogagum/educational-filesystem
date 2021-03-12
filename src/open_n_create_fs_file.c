#include "open_n_create_fs_file.h"

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
void*
create_fs_file(int* ret_fd,
               char* path,
               size_t inodes_cnt,
               size_t blocks_cnt)
{
    int fd = open(path, O_RDWR);

    if (fd == -1) 
    {   // Could not open file.
        return NULL;
    }

    size_t file_size = sizeof(struct fs_data) + 
                       inodes_cnt * sizeof(struct inode) + 
                       blocks_cnt * sizeof(BLOCK_SIZE * 1024);
    
    ftruncate(fd, file_size);

    void* maped_ptr = 
        mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        
    struct fs_data filesys_data = {.blocks_cnt = blocks_cnt,
                                   .blocks_stack_beginning = blocks_cnt,
                                   .blocks_tail_beginning = 0,
                                   .inodes_cnt = inodes_cnt,
                                   .inodes_stack_beginning = inodes_cnt,
                                   .inodes_tail_beginning = 0};

    set_filesys_data(&filesys_data, maped_ptr);

    if (maped_ptr != MAP_FAILED) 
    {   // Filed at mmap
        *ret_fd = fd;
        return maped_ptr;
    }
        
    close(fd);
    return NULL;
}

//----------------------------------------------------------------------------//
void* 
open_fs_file(int* ret_fd,
             char* path)
{
    int fd = open(path, O_RDWR | O_CREAT);

    if (fd == -1) 
    {   // Failed while opening
        return NULL;
    }

    struct stat st;
    fstat(fd, &st);

    void* mapped_ptr = 
        mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        
    if (mapped_ptr != MAP_FAILED) 
    {   // Filed at mmap
        ret_fd = &fd;
        return mapped_ptr;
    }

    close(fd);
    return NULL;
}