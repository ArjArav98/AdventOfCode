#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long line_calculate_sum_of_number_pair_products(const char* const line);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

unsigned long line_calculate_sum_of_number_pair_products(const char* const line) {
    regex_t regexp;
    regmatch_t matches[3];
    unsigned long product_sum = 0;

    if (regcomp(&regexp,
                "mul(\\([[:digit:]]\\{1,3\\}\\),\\([[:digit:]]\\{1,3\\}\\))",
                0) != 0) {
        return 0;
    }

    const char* line_copy = line;
    while (regexec(&regexp, line_copy, 3, matches, 0) == 0) {
        const unsigned long number1 = strtol(line_copy + matches[1].rm_so, NULL, 10);
        const unsigned long number2 = strtol(line_copy + matches[2].rm_so, NULL, 10);

        product_sum += (number1 * number2);
        line_copy += matches[0].rm_eo;
    }

    regfree(&regexp);
    return product_sum;
}

int main(void) {
    FILE* input_file = fopen("./input.txt", "r");
    unsigned long product_sum = 0;

    while (true) {
        const size_t LINE_LEN = 3100;
        char* const line = malloc(LINE_LEN * sizeof *line);
        fgets(line, LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        product_sum += line_calculate_sum_of_number_pair_products(line);
        free(line);
    }

    printf("Product is %lu\n", product_sum);

    fclose(input_file);
    return EXIT_SUCCESS;
}
