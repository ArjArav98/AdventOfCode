/* Day 03 - Part 1
 * Question Link: https://adventofcode.com/2025/day/3
 * Input Link: https://adventofcode.com/2025/day/3/input
 * */
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void array_init(short* const array, const size_t length);
void array_update(short* const array,
                  const size_t index,
                  const short value,
                  const size_t length);
long array_squash_numbers(const short* const array, const size_t length);
long highest_2dig_number_in_line(const char* const line);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

void array_init(short* const array, const size_t length) {
    for (size_t i = 0; i < length; i++)
        array[i] = -1;
}

void array_update(short* const array,
                  const size_t index,
                  const short value,
                  const size_t length) {
    array[index] = value;
    for (size_t i = index + 1; i < length; i++)
        array[i] = -1;
}

long array_squash_numbers(const short* const array, const size_t length) {
    long number = 0;
    for (size_t i = 0; i < length; i++)
        number = (number * 10) + (long)array[i];
    return number;
}

long highest_2dig_number_in_line(const char* const line) {
    const size_t HIGH_NUM_LEN = 2, line_len = strlen(line);

    short high_numbers[HIGH_NUM_LEN];
    array_init(high_numbers, HIGH_NUM_LEN);

    for (size_t line_i = 0; line_i < line_len; line_i++) {
        const short curr_number = c_to_s(line[line_i]);
        const size_t line_i_from_end = line_len - line_i - 1;
        const size_t comprsn_start_high_i =
            (line_i_from_end > HIGH_NUM_LEN) ? 0 : HIGH_NUM_LEN - line_i_from_end;

        for (size_t high_i = comprsn_start_high_i; high_i < HIGH_NUM_LEN; high_i++) {
            if (curr_number > high_numbers[high_i]) {
                array_update(high_numbers, high_i, curr_number, HIGH_NUM_LEN);
                break;
            }
        }
    }

    return array_squash_numbers(high_numbers, HIGH_NUM_LEN);
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    long highest_number_pair_sum = 0;

    while (true) {
        const size_t MAX_LINE_LENGTH = 110;
        char* const line = malloc((MAX_LINE_LENGTH + 1) * sizeof *line);

        fgets(line, MAX_LINE_LENGTH, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        highest_number_pair_sum += highest_2dig_number_in_line(line);
        free(line);
    }

    printf("The sum of the highest two-digit number in each line is %ld.\n",
           highest_number_pair_sum);

    fclose(input_file);
    return EXIT_SUCCESS;
}
