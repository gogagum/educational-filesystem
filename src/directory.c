/*
 * Returns ith internal file/directory
 */
void
get_ith_internal_file(size_t inode_index,
                      const struct fs_data* filesys_data, 
                      const void* mapped_file,
                      size_t inturnal_index,
                      struct link* ret_link)
{
    struct inode dir_inode;
    get_inode(inode_index, filesys_data, mapped_file, &dir_inode);
    assert(dir_inode.type == DIR);
    assert(inturnal_index * sizeof(link) < dir_inode.size);
    memcpy(ret_link, 
           get_ptr(inode_index, 
                   filesys_data, 
                   mapped_file, 
                   inturnal_index * sizeof(struct link)));
}