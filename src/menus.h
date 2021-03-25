#ifndef MENUS_H
#define MENUS_H

#include <stdio.h>
#include "open_n_create_fs_file.h"
#include "directory.h"
#include "reg_file.h"
#include "fs.h"

enum STARTUP_MENU_RESULT
{
    CREATE, 
    OPEN,
    STARTUP_MENU_ERROR
};

enum OPERATION_MENU_RESULT
{
    READ,
    WRITE,
    MKDIR,
    TOUCH, 
    LS,
    RM,
    EXIT,
    OPERATION_MENU_ERROR
};

/*
 * Gets from user what to do:
 * -to create new_file.
 * -open existing file.
 */
enum STARTUP_MENU_RESULT
startup_menu();

/* 
 * Gets path from user
 */
ssize_t
get_path_from_user(char** ret_path, 
                   size_t* n, 
                   char* message_str);

/*
 * Gets from user file name to open file.
 * Returns pointer to mapped_file and stats for file.
 */
void* 
open_menu(int* ret_fd,
          struct fs_data* filesys_data);

/*
 * Gets from user file name to create file.
 * Returns pointer to mapped_file and stats for file.
 */
void*
create_menu(int* ret_fd,
            struct fs_data* filesys_data);


/*
 * Working_loop.
 */
void
loop(int fd, 
     struct fs_data* filesys_data,
     void* mapped_file);

/*
 * Menu for choosing command.
 */
enum OPERATION_MENU_RESULT
command_choosing_menu();

/*
 * Gets filename from user and prints file.
 */
void 
read_cmd(struct fs_data* filesys_data,
         void* mapped_file);

/*
 * Gets filename and string from user and writes file.
 */
void 
write_cmd(struct fs_data* filesys_data,
          void* mapped_file);

/*
 * Gets filename and file name from user and creates file.
 */
void
makedir_cmd(struct fs_data* filesys_data, 
            void* mapped_file);

/*
 * Gets filename and file name from user and creates file.
 */
void
touch_menu(struct fs_data* filesys_data,
           void* mapped_file);

/*
 * Gets dir name from user and prints ls.
 */
void
ls_cmd(struct fs_data* filesys_data,
       void* mapped_file);

/*
 * Gets file name from user and removes file.
 */
void
rm_cmd(struct fs_data* filesys_data,
       void* mapped_file);


#endif  // MENUS_H
