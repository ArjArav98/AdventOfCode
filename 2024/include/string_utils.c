#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include"string_utils.h"

size_t char_occurs_in_string(char* const string, const char character) {
    size_t occurrences = 0;
    for (size_t index=0; index<strlen(string); index++) {
        if (string[index] == character) occurrences++;
    }
    return occurrences;
}

int split_string(
    char* const splits,
    const int nsplits,
    const char* const string,
    const char split_char
) {
    const size_t max_split_len = strlen(string) + 1;
    const size_t string_len = strlen(string);
    int split_count = 0;

    int offset = 0;
    for (int index=0; index < string_len; index++) {
        const bool max_splits_reached = split_count == nsplits;
        const bool curr_char_is_split_char = string[index] == split_char;
        const bool last_index = index == (string_len - 1);

        if (max_splits_reached) break;
        if (curr_char_is_split_char) {
            char* const destination_pointer = splits + (split_count * max_split_len);
            strncpy(destination_pointer, string + offset, index - offset + 1);
            *(destination_pointer + (index - offset)) = '\0';

            offset = index + 1;
            split_count++;
        }
        else if (last_index) {
            char* const destination_pointer = splits + (split_count * max_split_len);
            strncpy(destination_pointer, string + offset, string_len - offset);
            *(destination_pointer + (string_len - offset)) = '\0';

            split_count++;
        }
    }

    return split_count;
}

void string_list_to_num_list(
    long* num_list,
    char* string_list,
    int nstring_list_elements,
    size_t string_list_elem_size
) {
    for (int index=0; index < nstring_list_elements; index++) {
        num_list[index] = strtol(string_list + (index * string_list_elem_size), NULL, 10);
    }
}

void string_list_print(
    const char* const strings,
    size_t string_list_elem_size,
    size_t nstrings
) {
    for (int i=0; i<nstrings; i++) printf("%s ", strings + (string_list_elem_size * i));
    printf("\n");
}
