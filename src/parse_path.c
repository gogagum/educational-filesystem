#include "parse_path.h"

struct string_list_node*
add_string_to_list(struct string_list_node* head, char* str)
{
    struct string_list_node* new_head = malloc(sizeof(struct string_list_node));
    new_head->prev = head;
    new_head->str = strdup(str);
    return new_head;
}

struct string_list_node*
parse_path(char* str)
{
    struct string_list_node* last_node_ptr = 0;

    char* str_ptr = str;

    while (*str_ptr != 0) {
        // Get path or file str
        // Put this str to a list
    }

    return 0; /*ptr to last added node*/
}