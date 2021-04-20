#include "open_n_create_fs_file.h"

//----------------------------------------------------------------------------//
void*
create_fs_file(int* ret_fd,
               char* path,
               size_t inodes_cnt,
               size_t blocks_cnt)
{
    int fd = open(path, O_RDWR | O_CREAT, 0666);

    if (fd == -1) 
    {    
        return NULL;
    }

    size_t file_size = BLOCKS_INFO_SECTION_SIZE * BYTES_BLOCK_SIZE + 
                       inodes_cnt * sizeof(struct inode) + 
                       blocks_cnt * BYTES_BLOCK_SIZE;
    
    ftruncate(fd, file_size);

    void* mapped_file = 
        mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        
    struct fs_data filesys_data = {.blocks_cnt = blocks_cnt,
                                   .blocks_stack_beginning = blocks_cnt,
                                   .blocks_tail_beginning = 0,
                                   .inodes_cnt = inodes_cnt,
                                   .inodes_stack_beginning = inodes_cnt,
                                   .inodes_tail_beginning = 0};

    
    if (mapped_file != MAP_FAILED) 
    {   
        set_filesys_data(&filesys_data, mapped_file);
        *ret_fd = fd;
        return mapped_file;
    }

    // Failed at mmap

    close(fd);
    return NULL;
}

//----------------------------------------------------------------------------//
void* 
open_fs_file(int* ret_fd,
             char* path)
{
    int fd = open(path, O_RDWR, 0666);

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
        *ret_fd = fd;
        return mapped_ptr;
    }

    close(fd);
    return NULL;
}
