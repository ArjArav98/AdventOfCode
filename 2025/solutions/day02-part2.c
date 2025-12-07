/* Day 02 - Part 2
 * Question Link: https://adventofcode.com/2025/day/2
 * Input Link: https://adventofcode.com/2025/day/2/input
 * */
#include "map.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long number_of_digits(const long number);
long generate_repeated_digit_number(const long number, const long repeats);
long sum_of_repeated_digit_numbers(const long range_start, const long range_end);

/*----------------*/
/* IMPLEMENTATION */
/*----------------*/

long number_of_digits(const long number) {
    long digit_count = 0;
    for (long number_copy = number; number_copy != 0; number_copy /= 10)
        digit_count++;
    return digit_count;
}

long generate_repeated_digit_number(const long number, const long repeats) {
    const long number_ndigits = number_of_digits(number);
    long gen_number = 0;

    for (long i = 0; i < repeats; i++)
        gen_number = gen_number * (long)pow(10, number_ndigits) + number;

    return gen_number;
}

long sum_of_repeated_digit_numbers(const long range_start, const long range_end) {
    const long range_end_ndigits = number_of_digits(range_end);

    long sum_of_repeated_digit_numbers = 0;
    struct Map* const map = map_create(-1);
    map_insert(map, 0, 0);

    // We generate a number in this loop TO BE repeated.
    for (long number_i = 1; (number_of_digits(number_i) <= range_end_ndigits / 2);
         number_i++) {

        // We generate numbers with the NUMBER^ repeated.
        for (long repeat_i = number_of_digits(range_start) / number_of_digits(number_i);
             true;
             repeat_i++) {
            if (repeat_i == 1) repeat_i = 2; // Minimum repeats of 2

            const long gen_number = generate_repeated_digit_number(number_i, repeat_i);
            if (gen_number > range_end) break;

            if ((gen_number >= range_start) && (gen_number <= range_end)) {
                if (map_get(map, gen_number) == -1) {
                    sum_of_repeated_digit_numbers += gen_number;
                    map_insert(map, gen_number, 0);
                }
            }
        }
    }

    map_free(map);
    return sum_of_repeated_digit_numbers;
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
            sum_of_repeated_digit_numbers(range_start, range_end);
    }

    printf("The sum of numbers with repeated digits is %ld\n",
           repeated_digit_number_sum);

    free(input);
    fclose(input_file);
    return EXIT_SUCCESS;
}
