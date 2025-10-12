#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long
line_calculate_sum_of_number_pair_products(const char* const line,
                                           bool* const count_current_product);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

unsigned long
line_calculate_sum_of_number_pair_products(const char* const line,
                                           bool* const count_current_product) {
    regex_t num_mul_regexp, do_regexp, dont_regexp;
    regmatch_t num_mul_matches[3], do_matches[1], dont_matches[1];
    unsigned long product_sum = 0;

    if (regcomp(&num_mul_regexp,
                "mul(\\([[:digit:]]\\{1,3\\}\\),\\([[:digit:]]\\{1,3\\}\\))",
                0) != 0) {
        return 0;
    }
    if (regcomp(&do_regexp, "do()", 0) != 0) {
        regfree(&num_mul_regexp);
        return 0;
    }
    if (regcomp(&dont_regexp, "don't()", 0) != 0) {
        regfree(&num_mul_regexp);
        regfree(&do_regexp);
        return 0;
    }

    const char* line_copy = line;
    while (regexec(&num_mul_regexp, line_copy, 3, num_mul_matches, 0) == 0) {
        const unsigned long number1 =
            strtol(line_copy + num_mul_matches[1].rm_so, NULL, 10);
        const unsigned long number2 =
            strtol(line_copy + num_mul_matches[2].rm_so, NULL, 10);

        const bool dont_regexp_found =
            (regexec(&dont_regexp, line_copy, 1, dont_matches, 0) == 0) &&
            (dont_matches[0].rm_so < num_mul_matches[0].rm_so);
        const bool do_regexp_found =
            (regexec(&do_regexp, line_copy, 1, do_matches, 0) == 0) &&
            (do_matches[0].rm_so < num_mul_matches[0].rm_so);

        const regoff_t dont_start_offset = dont_matches[0].rm_so;
        const regoff_t do_start_offset = do_matches[0].rm_so;

        if (dont_regexp_found && do_regexp_found)
            *count_current_product = do_start_offset > dont_start_offset;
        else if (dont_regexp_found)
            *count_current_product = false;
        else if (do_regexp_found)
            *count_current_product = true;

        if (*count_current_product == true) product_sum += (number1 * number2);
        line_copy += num_mul_matches[0].rm_eo;
    }

    regfree(&num_mul_regexp);
    regfree(&do_regexp);
    regfree(&dont_regexp);
    return product_sum;
}

int main(void) {
    FILE* input_file = fopen("./input.txt", "r");
    unsigned long product_sum = 0;
    bool count_product_in_sum = true;

    while (true) {
        const size_t LINE_LEN = 3100;
        char* const line = malloc(LINE_LEN * sizeof *line);
        fgets(line, LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        product_sum +=
            line_calculate_sum_of_number_pair_products(line, &count_product_in_sum);
        free(line);
    }

    printf("Product is %lu\n", product_sum);

    fclose(input_file);
    return EXIT_SUCCESS;
}
