#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include"string_utils.h"


struct Rule;
struct Rule rule_parse_from_line(char* const line);
void rules_print(struct Rule* const rules, int nrules);
void ordered_page_numbers_add_number(
    long* const ordered_page_numbers,
    const size_t size,
    const long number
);
void ordered_page_numbers_update_order_for_rule(
    long* const ordered_page_numbers,
    const size_t size,
    const struct Rule rule
);
void ordered_page_numbers_print(
    const long* const ordered_page_numbers,
    const size_t size
);
size_t ordered_page_numbers_get_true_size(
    long* const ordered_page_numbers,
    size_t size
);
int line_to_page_update_list(
    long* num_list,
    size_t num_list_size,
    const char* const line
);
bool page_list_contains_rule_numbers(
    const long* const num_list,
    const size_t size,
    struct Rule rule
);
void page_update_list_print(const long* const num_list, const size_t size);


struct Rule {
    int before_num;
    int after_num;
};

struct Rule rule_parse_from_line(char* const line) {
    struct Rule rule = { .before_num = 0, .after_num = 0 };
    sscanf(line, "%d|%d", &rule.before_num, &rule.after_num);
    return rule;
}

void rules_print(struct Rule* const rules, int nrules) {
    for (int i=0; i<nrules; i++)
        printf("%d|%d\n", rules[i].before_num, rules[i].after_num);
}

void ordered_page_numbers_add_number(
    long* const ordered_page_numbers,
    const size_t size,
    const long number
) {
    for (int index=0; index < size; index++) {
        if (ordered_page_numbers[index] == number) break;
        else if (ordered_page_numbers[index] == 0) {
            ordered_page_numbers[index] = number;
            break;
        }
    }
}

void ordered_page_numbers_update_order_for_rule(
    long* const ordered_page_numbers,
    const size_t size,
    const struct Rule rule
) {
    int before_num_index = 0, after_num_index = 0;

    for (int index=0; index < size; index++) {
        if (ordered_page_numbers[index] == rule.before_num) before_num_index = index;
        if (ordered_page_numbers[index] == rule.after_num) after_num_index = index;
    }

    if (before_num_index < after_num_index) return;

    for (int index=size-1; index >=0; index--) {
        const bool index_contains_nothing = ordered_page_numbers[index] == 0;
        const bool index_before_before_num_index = (index < before_num_index);

        if (index_contains_nothing) continue;
        if (index_before_before_num_index)
            ordered_page_numbers[index+1] = ordered_page_numbers[index];
        if (index == after_num_index) {
            ordered_page_numbers[index] = rule.before_num;
            break;
        }
    }
}

void ordered_page_numbers_print(
    const long* const ordered_page_numbers,
    const size_t size
) {
    for (int i=0; (i<size) && (ordered_page_numbers[i] != 0); i++)
        printf("%ld ", ordered_page_numbers[i]);
    printf("\n");
}

size_t ordered_page_numbers_get_true_size(
    long* const ordered_page_numbers,
    size_t size
) {
    size_t true_size = 0;
    for (int i=0; i<size && (ordered_page_numbers[i] != 0); i++) true_size++;
    return true_size;
}

int line_to_page_update_list(
    long* num_list,
    size_t num_list_size,
    const char* const line
) {
    const size_t string_list_elem_len = strlen(line) + 1;
    const size_t string_list_size = num_list_size;

    char* const string_list = malloc(
        sizeof(char) * string_list_size * string_list_elem_len
    );
    int nsplits = split_string(
        string_list,
        string_list_size,
        line,
        ','
    );
    string_list_to_num_list(num_list, string_list, nsplits, string_list_elem_len);

    free(string_list);
    return nsplits;
}

bool page_list_contains_rule_numbers(
    const long* const num_list,
    const size_t size,
    struct Rule rule
) {
    bool before_exists = false, after_exists = false;

    for (int i=0; i<size; i++) {
        if (num_list[i] == rule.before_num) before_exists = true;
        if (num_list[i] == rule.after_num) after_exists = true;
    }

    return before_exists && after_exists;
}

void page_update_list_print(const long* const num_list, const size_t size) {
    for (int i=0; i<size; i++) printf("%ld,", num_list[i]);
    printf("\n");
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const int MAX_RULES = 1176;
    struct Rule* const rules = malloc(sizeof(struct Rule) * MAX_RULES);
    
    long middle_number_sum = 0;
    int nrules = 0;

    while (true) {
        const int MAX_LINE_LEN = 7;
        char* const line = malloc(sizeof(char) * MAX_LINE_LEN);
        fgets(line, MAX_LINE_LEN, input_file);

        if (strlen(line) == 1) {
            free(line);
            break;
        }
       
        rules[nrules] = rule_parse_from_line(line);
        nrules++;

        free(line);
    }

    while (true) {
        const int MAX_LINE_LEN = 100;
        char* const line = malloc(sizeof(char) * MAX_LINE_LEN);
        fgets(line, MAX_LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        // We get the page update list
        const size_t comma_occurrences = char_occurs_in_string(line, ',');
        const size_t page_update_list_size = comma_occurrences + 1;
        long* const page_update_list = calloc(page_update_list_size, sizeof(long));
        
        const int page_update_list_elems = line_to_page_update_list(
            page_update_list,
            page_update_list_size,
            line
        );
       
        // We order the page numbers
        const size_t nnumbers = nrules;
        long* ordered_page_numbers = calloc(nnumbers, sizeof(long));

        for (int index=0; index < nrules; index++) {
            if(
                page_list_contains_rule_numbers(
                    page_update_list,
                    page_update_list_elems,
                    rules[index]
                )
            ) {
                ordered_page_numbers_add_number(ordered_page_numbers, nnumbers, rules[index].before_num);
                ordered_page_numbers_add_number(ordered_page_numbers, nnumbers, rules[index].after_num);
                ordered_page_numbers_update_order_for_rule(ordered_page_numbers, nnumbers, rules[index]);
            }
        }

        for (int index=nrules; index >= 0; index--) {
            if(
                page_list_contains_rule_numbers(
                    page_update_list,
                    page_update_list_elems,
                    rules[index]
                )
            ) {
                ordered_page_numbers_add_number(ordered_page_numbers, nnumbers, rules[index].before_num);
                ordered_page_numbers_add_number(ordered_page_numbers, nnumbers, rules[index].after_num);
                ordered_page_numbers_update_order_for_rule(ordered_page_numbers, nnumbers, rules[index]);
            }
        }

        // We determine which page updates are correctly ordered
        // by checking if both arrays are equal
        size_t ordered_page_numbers_true_size = ordered_page_numbers_get_true_size(
            ordered_page_numbers,
            nnumbers
        );
        if (page_update_list_elems == ordered_page_numbers_true_size) {
            const size_t common_size = ordered_page_numbers_true_size;
            bool is_equal = true;

            for (int index=0; index < common_size; index++) {
                if (page_update_list[index] != ordered_page_numbers[index]) {
                    is_equal = false;
                    break;
                }
            }

            if (is_equal) {
                const size_t middle_element = (page_update_list_size / 2);
                middle_number_sum += page_update_list[middle_element];
            }
        }

        free(ordered_page_numbers);
        free(page_update_list);
        free(line);
    }

    printf("The sum of middle elements of lists with correct order is %ld\n", middle_number_sum);

    free(rules);
    fclose(input_file);
    return EXIT_SUCCESS;
}
