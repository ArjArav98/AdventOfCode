/* Day 02 - Part 1
 * Question Link: https://adventofcode.com/2025/day/2
 * Input Link: https://adventofcode.com/2025/day/2/input
 * */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

short number_of_digits(const long number);
long sum_of_repeated_digit_numbers_in_range(const long range_start,
                                            const long range_end);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

short number_of_digits(const long number) {
    short digit_count = 0;
    for (long number_copy = number; number_copy != 0; number_copy /= 10)
        digit_count++;
    return digit_count;
}

long sum_of_repeated_digit_numbers_in_range(const long range_start,
                                            const long range_end) {
    const short range_start_ndigits = number_of_digits(range_start);
    const short range_end_ndigits = number_of_digits(range_end);

    const long new_range_start = (range_start_ndigits % 2 == 0)
                                     ? range_start
                                     : (long)pow(10, range_start_ndigits);
    const long new_range_end = (range_end_ndigits % 2 == 0)
                                   ? range_end
                                   : ((long)pow(10, range_end_ndigits - 1) - 1);

    const short new_range_start_ndigits = number_of_digits(new_range_start);

    const long number_generation_start =
        range_start / (long)pow(10, new_range_start_ndigits / 2);

    long repeated_digit_number_sum = 0;

    for (long i = number_generation_start; true; i++) {
        const long repeated_number =
            (i * (long)pow(10, new_range_start_ndigits / 2)) + i;

        if ((repeated_number >= new_range_start) && (repeated_number <= new_range_end))
            repeated_digit_number_sum += repeated_number;

        if (repeated_number > new_range_end) break;
    }

    return repeated_digit_number_sum;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const size_t MAX_LINE_LENGTH = 500;
    char* const input = malloc(MAX_LINE_LENGTH * sizeof *input);
    fgets(input, MAX_LINE_LENGTH, input_file);

    long repeated_digit_number_sum = 0;

    for (char* search_str = input; strlen(search_str) != 0; search_str++) {
        char* hyphen_start = NULL;
        const long range_start = strtol(search_str, &hyphen_start, 10);
        const long range_end = strtol(hyphen_start + 1, &search_str, 10);

        repeated_digit_number_sum +=
            sum_of_repeated_digit_numbers_in_range(range_start, range_end);
    }

    printf("The sum of numbers with repeated digits is %ld\n",
           repeated_digit_number_sum);

    free(input);
    fclose(input_file);
    return EXIT_SUCCESS;
}
