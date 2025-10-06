#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func_cache.h"
#include "string_utils.h"

int number_digit_count(long number);

long generate_blink_function_param_cache_key(const long parameter_1,
                                             const short parameter_2);

long stones_after_blink(long stone_number,
                        short curr_depth,
                        struct FuncCache* const func_cache);

long number_digits_half(const long number,
                        const long number_ndigits,
                        const bool left_half);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

long generate_blink_function_param_cache_key(const long parameter_1,
                                             const short parameter_2) {
    return (parameter_1 * 100) + (long)parameter_2;
}

int number_digit_count(const long number) {
    int digits_length = 1;

    for (unsigned long ten_power = 1; (number / (long)pow(10, ten_power)) != 0;
         ten_power++, digits_length++)
        ;

    return digits_length;
}

long number_digits_half(const long number,
                        const long number_ndigits,
                        const bool left_half) {
    long half_ndigits = number_ndigits / 2;
    return (left_half) ? number / (long)pow(10, half_ndigits)
                       : number % (long)pow(10, half_ndigits);
}

long stones_after_blink(const long stone_number,
                        const short curr_depth,
                        struct FuncCache* const func_cache) {
    if (curr_depth == 0) return 1;

    const long cache_key =
        generate_blink_function_param_cache_key(stone_number, curr_depth);
    const long cached_result = func_cache_cached_result(func_cache, cache_key);
    const bool func_cache_has_result = cached_result != FUNC_CACHE_NO_RESULT;
    if (func_cache_has_result) return cached_result;

    long nstones = 0;
    const int number_ndigits = number_digit_count(stone_number);

    if (stone_number == 0)
        nstones += stones_after_blink(1, curr_depth - 1, func_cache);
    else if (number_ndigits % 2 == 0) {
        nstones +=
            stones_after_blink(number_digits_half(stone_number, number_ndigits, true),
                               curr_depth - 1,
                               func_cache);
        nstones +=
            stones_after_blink(number_digits_half(stone_number, number_ndigits, false),
                               curr_depth - 1,
                               func_cache);
    } else
        nstones += stones_after_blink(stone_number * 2024, curr_depth - 1, func_cache);

    if (!func_cache_has_result) func_cache_add_result(func_cache, cache_key, nstones);

    return nstones;
}

int main(void) {
    FILE* input_file = fopen("./input.txt", "r");

    const size_t MAX_LINE_LEN = 100;
    char* const line = malloc(sizeof(char) * MAX_LINE_LEN);

    // Get input as line
    fgets(line, MAX_LINE_LEN, input_file);
    const size_t max_nsplits = char_occurs_in_string(line, ' ') + 1,
                 max_split_len = strlen(line) + 1;
    char* const line_splits = malloc(sizeof(char) * max_split_len * max_nsplits);
    const size_t nsplits = split_string(line_splits, max_nsplits, line, ' ');

    // Parse line into number list
    long* const num_list = calloc(nsplits, sizeof(long));
    string_list_to_num_list(num_list, line_splits, nsplits, max_split_len);

    // Compute
    const short BLINKS = 75;
    struct FuncCache* blink_func_cache;
    func_cache_initialise(&blink_func_cache);

    long total_stones = 0;
    for (size_t i = 0; i < nsplits; i++) {
        total_stones += stones_after_blink(num_list[i], BLINKS, blink_func_cache);
    }

    printf(
        "The total number of stones after %hd blinks is %ld\n", BLINKS, total_stones);

    func_cache_free(blink_func_cache);
    free(num_list);
    free(line_splits);
    free(line);
    fclose(input_file);
    return EXIT_SUCCESS;
}
