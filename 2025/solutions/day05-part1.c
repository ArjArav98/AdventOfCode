/* Day 05 - Part 1
 * Question Link: https://adventofcode.com/2025/day/5
 * Input Link: https://adventofcode.com/2025/day/5/input
 * */
#include "binary_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Range;

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

struct Range {
    unsigned long start;
    unsigned long end;
};

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const size_t NRANGES = 185, BIN_TREE_NO_RESULT_VALUE = NRANGES + 1;
    struct BinaryTree* const binary_tree_map =
        binary_tree_create(BIN_TREE_NO_RESULT_VALUE);
    struct Range ranges[NRANGES];

    for (size_t i = 0; true; i++) {
        const size_t LINE_LENGTH = 50;
        char* const line = malloc(LINE_LENGTH * sizeof *line);

        fgets(line, LINE_LENGTH, input_file);

        if (strlen(line) == 1) {
            free(line);
            break;
        }

        // We use an array for ease of retrieval and...
        char* end_of_first_num = NULL;
        ranges[i] = (struct Range){.start = strtoul(line, &end_of_first_num, 10),
                                   .end = strtoul(end_of_first_num + 1, NULL, 10)};

        // ... a binary tree for getting the array index quickly.
        const long long existing_bin_tree_value =
            binary_tree_get_key_value(binary_tree_map, ranges[i].start);
        if ((existing_bin_tree_value == (long long)BIN_TREE_NO_RESULT_VALUE) ||
            (ranges[existing_bin_tree_value].end < ranges[i].end))
            binary_tree_insert(binary_tree_map, ranges[i].start, i);

        free(line);
    }

    short number_in_ranges_count = 0;
    while (true) {
        const size_t LINE_LENGTH = 50;
        char* const line = malloc(LINE_LENGTH * sizeof *line);

        fgets(line, LINE_LENGTH, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        const unsigned long number = strtoul(line, NULL, 10);

        // We get the closest previous range start for the number.
        // If not in that range, we try the next range with the closest prev start.
        for (unsigned long range_start = number; true;) {
            const size_t range_i =
                (size_t)binary_tree_get_closest_key_value(binary_tree_map, range_start);
            if (range_i == BIN_TREE_NO_RESULT_VALUE) break;

            if ((ranges[range_i].start <= number) && (ranges[range_i].end >= number)) {
                number_in_ranges_count++;
                break;
            }

            range_start = ranges[range_i].start - 1;
        }

        free(line);
    }

    printf("The count of numbers present in the ranges is %hd\n",
           number_in_ranges_count);

    binary_tree_free(binary_tree_map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
