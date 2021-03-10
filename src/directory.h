/*
 * Record about file in direcory
 */
struct dir_line
{
    uint16_t inode_index;  // Inode index of a file (or directory file) inside directory
    char[14] file_name;    // Name of a file (or directory file) inside directory
}
