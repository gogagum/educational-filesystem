#ifndef MENUS_H
#define MENUS_H

#include <stdio.h>
#include "open_n_create_fs_file.h"

enum STARTUP_MENU_RESULT
{
    CREATE, 
    OPEN,
    ERROR
};

enum OPERATION_MENU_RESULT
{
    READ,
    WRITE,
    LS,
    CD,
    RM
};

/*
 * Gets from user what to do:
 * -to create new_file.
 * -open existing file.
 */
enum STARTUP_MENU_RESULT
startup_menu();

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
create_menu(struct fs_data* filesys_data);

#endif  // MENUS_H