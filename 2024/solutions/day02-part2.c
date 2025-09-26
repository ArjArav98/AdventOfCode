#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include"string_utils.h"

long** convert_line_to_num_list(char* line);
long** copy_num_list_without_element(long** num_list, const size_t word_count, size_t element);
void print_num_list(long** num_list, const size_t size);
void free_num_list(long** num_list, const size_t size);
bool greater_than(long a, long b);
bool smaller_than(long a, long b);
bool diff_within_threshold(long a, long b);
bool all_true_for_successive_num_list_elements(
    long** num_list,
    const size_t size,
    bool (*op)(long, long)
);
bool is_level_safe(long** num_list, const size_t size);


long** convert_line_to_num_list(char* line) {
    size_t word_count = char_occurs_in_string(line, ' ') + 1;
    char** words = malloc(sizeof(char*) * word_count);
    long** num_list = malloc(sizeof(long*) * word_count);

    split_string(words, line, ' ');

    for (size_t index=0; index<word_count; index++) {
        num_list[index] = malloc(sizeof(long));
        *(num_list[index]) = strtol(words[index], NULL, 10);
    }

    free_string_array(words, word_count);
    return num_list;
}

long** copy_num_list_without_element(long** num_list, const size_t word_count, size_t element) {
    long** copy_num_list = malloc(sizeof(long*) * (word_count-1));
    for (size_t new_list_index=0, old_list_index=0; new_list_index<word_count-1; new_list_index++, old_list_index++) {
        copy_num_list[new_list_index] = malloc(sizeof(long));
        if (new_list_index == element) old_list_index++;
        *(copy_num_list[new_list_index]) = *(num_list[old_list_index]);
    }
    return copy_num_list;
}

bool greater_than(long a, long b) {
    return a > b;
}

bool smaller_than(long a, long b) {
    return a < b;
}

bool diff_within_threshold(long a, long b) {
    int difference = abs((int)a - (int)b);
    return ((difference >= 1) && (difference <= 3));
}

bool all_true_for_successive_num_list_elements(
    long** num_list,
    const size_t size,
    bool (*op)(long, long)
) {
    for (size_t index=1; index < size; index++) {
        if (!op(*num_list[index-1], *num_list[index])) return false;
    }
    return true;
}

bool is_level_safe(long** num_list, const size_t size) {
    return (
        (
            all_true_for_successive_num_list_elements(num_list, size, greater_than) &&
            all_true_for_successive_num_list_elements(num_list, size, diff_within_threshold)
        ) || (
            all_true_for_successive_num_list_elements(num_list, size, smaller_than) &&
            all_true_for_successive_num_list_elements(num_list, size, diff_within_threshold)
        )
    );
}

void print_num_list(long** num_list, const size_t size) {
    for (size_t index=0; index<size; index++) printf("%ld ", *(num_list[index]));
}

void free_num_list(long** num_list, const size_t size) {
    for (size_t index=0; index<size; index++) free(num_list[index]);
    free(num_list);
}

int main(void) {
    int safe_level_count = 0;
    FILE* input_file = fopen("./input/day02.txt", "r");

    if (input_file == NULL) {
        perror("Error reading input file");
        return EXIT_FAILURE;
    }

    while(true) {
        const int line_size = 50;
        char line[line_size];
        fgets(line, line_size, input_file);

        if (feof(input_file) != false) break;
        
        const size_t word_count = char_occurs_in_string(line, ' ') + 1;
        long** num_list = convert_line_to_num_list(line);

        if (is_level_safe(num_list, word_count)) safe_level_count++;
        else {
            for (size_t index=0; index < word_count; index++) {
                long** new_num_list = copy_num_list_without_element(num_list, word_count, index);
                
                if (is_level_safe(new_num_list, word_count-1)) {
                    safe_level_count++;
                    index=word_count;
                }
                free(new_num_list);
            }
        }

        free_num_list(num_list, word_count);
    }

    printf("%d number of safe levels.\n", safe_level_count);
    return EXIT_SUCCESS;
}
