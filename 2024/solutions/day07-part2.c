#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<regex.h>
#include"string_utils.h"

size_t extract_string_nums_from_line(
    char* const string_num_list,
    const size_t max_string_num_len,
    const char* const line
);
long combine_two_numbers_in_order(const long left_num, const long right_num);
bool is_expression_valid(
    const long* const numbers,
    const size_t size,
    const long current_result,
    const long final_result
);
void num_list_print(const long* const numbers, size_t size);


size_t extract_string_nums_from_line(
    char* const string_num_list,
    const size_t max_string_num_len,
    const char* const line
) {
    regex_t regexp;
    if (regcomp(&regexp, "[[:digit:]]\\{1,\\}", 0) != 0) {
        perror("Regex not compiling");
        return 0;
    }

    regmatch_t matches[1];
    int nstrings_extracted = 0;
    const char* line_copy = line;

    while (regexec(&regexp, line_copy, 1, matches, 0) == 0) {
        size_t row_index = max_string_num_len * nstrings_extracted;
        int extracted_string_len = matches[0].rm_eo - matches[0].rm_so;

        strncpy(string_num_list + row_index, line_copy + matches[0].rm_so, extracted_string_len);
        *(string_num_list + row_index + extracted_string_len) = '\0';
        
        line_copy += matches[0].rm_eo;
        nstrings_extracted++;
    }

    regfree(&regexp);
    return nstrings_extracted;
}

long combine_two_numbers_in_order(const long left_num, const long right_num) {
    if (right_num == 0) return (left_num * 10) + right_num;

    long power_of_10 = 10;
    for (; (right_num / power_of_10) != 0; power_of_10 *= 10);

    return (left_num * power_of_10) + right_num;
}

bool is_expression_valid(
    const long* const numbers,
    const size_t size,
    const long current_result,
    const long final_result
) {
    if (size == 0) {
        return current_result == final_result;
    };

    return (
        is_expression_valid(numbers + 1, size - 1, current_result + numbers[0], final_result) ||
        is_expression_valid(numbers + 1, size - 1, current_result * numbers[0], final_result) ||
        is_expression_valid(numbers + 1, size - 1, combine_two_numbers_in_order(current_result, numbers[0]), final_result)
    );
}

void num_list_print(const long* const numbers, size_t size) {
    for (int i=0; i<size; i++) printf("%ld ", numbers[i]);
    printf("\n");
}

int main(void) {
    FILE* const input_file = fopen("./input/day07.txt", "r");
    long sum_of_valid_expression_first_element = 0;

    while(true) {
        const size_t MAX_LINE_LEN = 50;
        char* const line = malloc(sizeof(char) * MAX_LINE_LEN);
        fgets(line, MAX_LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }
        
        const size_t nlist_numbers = MAX_LINE_LEN / 2;
        const size_t number_string_len = (MAX_LINE_LEN / 2) + 1;
        char* const string_num_list = malloc(sizeof(char) * nlist_numbers * number_string_len);
        const size_t nstrings = extract_string_nums_from_line(string_num_list, number_string_len, line);
        
        long* const num_list = malloc(sizeof(long) * nstrings);
        string_list_to_num_list(num_list, string_num_list, nstrings, number_string_len);
        
        if (is_expression_valid(num_list+2, nstrings - 2, num_list[1], num_list[0]))
            sum_of_valid_expression_first_element += num_list[0];

        free(num_list);
        free(string_num_list);
        free(line);
    }

    printf("Total valid expressions are %ld\n", sum_of_valid_expression_first_element);

    fclose(input_file);
    return EXIT_SUCCESS;
}
