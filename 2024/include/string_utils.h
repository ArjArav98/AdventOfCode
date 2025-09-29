#ifndef STRINGUTILS_H
#define STRINGUTILS_H

size_t char_occurs_in_string(char* string, char split_char);
int split_string(
    char* const splits,
    const int nsplits,
    const char* const string,
    const char split_char
);
void string_list_to_num_list(
    long* num_list,
    char* string_list,
    int nstring_list_elements,
    size_t string_list_elem_size
);

#endif /* STRINGUTILS_H */
