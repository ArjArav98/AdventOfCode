#include "binary_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void num_list_from_line(short* const num_list, const char* const line);
bool num_list_is_ordered(const short* const num_list,
                         const size_t nnums,
                         const struct BinaryTreeNode num_rule_trees[100]);
bool num_list_n_elements_are_present_in_num_rule_tree(
    const short* const num_list,
    const size_t nnums,
    const struct BinaryTreeNode num_rule_tree,
    const unsigned long n);

void num_list_from_line(short* const num_list, const char* const line) {
    const size_t nnums = (strlen(line) + 1) / 3;
    char* end_of_number = NULL;

    num_list[0] = (short)strtol(line, &end_of_number, 10);

    for (size_t i = 1; i < nnums; i++) {
        num_list[i] = (short)strtol(end_of_number + 1, &end_of_number, 10);
    }
}

bool num_list_is_ordered(const short* const num_list,
                         const size_t nnums,
                         const struct BinaryTreeNode num_rule_trees[100]) {

    for (size_t i = 0; i < nnums; i++)
        for (size_t j = i + 1; j < nnums; j++)
            if (!binary_tree_value_exists(&num_rule_trees[num_list[i]], num_list[j]))
                return false;

    return true;
}

bool num_list_n_elements_are_present_in_num_rule_tree(
    const short* const num_list,
    const size_t nnums,
    const struct BinaryTreeNode num_rule_tree,
    const unsigned long n) {

    unsigned long num_list_elements_in_num_rule_tree = 0;
    for (size_t i = 0; i < nnums; i++) {
        if (binary_tree_value_exists(&num_rule_tree, num_list[i]))
            num_list_elements_in_num_rule_tree++;
    }

    return num_list_elements_in_num_rule_tree == n;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    struct BinaryTreeNode num_rule_trees[100];
    for (size_t i = 0; i < 100; i++)
        binary_tree_init(&num_rule_trees[i]);

    while (true) {
        const size_t LINE_LEN = 10;
        char* const line = malloc(LINE_LEN * sizeof *line);
        fgets(line, LINE_LEN, input_file);

        if (strlen(line) == 1) {
            free(line);
            break;
        }

        char* end_of_number = NULL;
        const long number1 = strtol(line, &end_of_number, 10);
        const long number2 = strtol(end_of_number + 1, NULL, 10);

        binary_tree_add(&num_rule_trees[number1], number2);

        free(line);
    }

    unsigned long middle_elem_sum = 0;
    while (true) {
        const size_t LINE_LEN = 100;
        char* const line = malloc(LINE_LEN * sizeof *line);
        fgets(line, LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        const size_t nnums = (strlen(line) + 1) / 3;
        short* const num_list = calloc(nnums, sizeof *num_list);
        num_list_from_line(num_list, line);

        /* For any element in this ordered list, the number of other elements
         * present in its rule list is equal to N - i - 1, where N is the total
         * number of elements and i is the index of the element itself.
         *
         * For eg; 75,47, 61, 53, 29 is an ordered list.
         * The 0th element has rules present for (5 - 0 - 1) elements, ie, the
         * elements following it.
         * Similarly, the middle element should have (N - N/2 - 1) or N/2 elements
         * matching. */

        if (!num_list_is_ordered(num_list, nnums, num_rule_trees)) {
            const unsigned long middle_elem = nnums / 2,
                                nmatching_elements = middle_elem;

            for (size_t i = 0; i < nnums; i++) {
                if (num_list_n_elements_are_present_in_num_rule_tree(
                        num_list,
                        nnums,
                        num_rule_trees[num_list[i]],
                        nmatching_elements)) {
                    middle_elem_sum += (unsigned long)num_list[i];
                    break;
                }
            }
        }

        free(num_list);
        free(line);
    }

    printf("The sum of middle elements of correctly-ordered lists is %ld.\n",
           middle_elem_sum);

    for (size_t i = 0; i < 100; i++)
        binary_tree_free(&num_rule_trees[i]);
    fclose(input_file);
    return EXIT_SUCCESS;
}
