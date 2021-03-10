#ifndef PARSE_PATH_H
#define PARSE_PATH_H

struct string_list_node*
add_string_to_list(struct string_list_node* head, char* str);

struct string_list_node*
parse_path(char* str);

#endif  // PARSE_PATH_H