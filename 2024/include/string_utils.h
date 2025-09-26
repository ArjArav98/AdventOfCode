#ifndef STRINGUTILS_H
#define STRINGUTILS_H

void split_string(char** destination, char* string, char split_char);
size_t char_occurs_in_string(char* string, char split_char);
void free_string_array(char** string_array, size_t size);

#endif /* STRINGUTILS_H */
