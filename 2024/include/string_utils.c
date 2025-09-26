#include<string.h>
#include<stdlib.h>
#include"string_utils.h"

size_t char_occurs_in_string(char* string, char split_char) {
    size_t occurrences = 0;
    for (size_t index=0; index<strlen(string); index++) {
        if (string[index] == split_char) occurrences++;
    }
    return occurrences;
}

void split_string(char** destination, char* string, char split_char) {
    size_t occurrences = char_occurs_in_string(string, split_char);

    for (size_t index=0; index<(occurrences+1); index++) {
        destination[index] = malloc(sizeof(char)*(strlen(string)+1));
    }

    size_t destination_index=0;
    size_t destination_index_char=0;

    for (size_t index=0; index<=strlen(string); index++) {
        if ((index == strlen(string)) || (string[index] == split_char)) {
            destination[destination_index][destination_index_char] = '\0';
            destination_index++;
            destination_index_char=0;
        } else {
            destination[destination_index][destination_index_char] = string[index];
            destination_index_char++;
        }
    }
}

void free_string_array(char** string_array, size_t size) {
    for (size_t index=0; index<size; index++) free(string_array[index]);
    free(string_array);
}
