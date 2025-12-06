/* Day 06 - Part 1
 * Question Link: https://adventofcode.com/2025/day/6
 * Input Link: https://adventofcode.com/2025/day/6/input
 * */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Matrix;
long long add(long long number1, long long number2);
long long multiply(long long number1, long long number2);
long long matrix_operation_on_column(const struct Matrix matrix,
                                     const size_t column,
                                     long long (*operator)(long long, long long),
                                     const long long initial_value);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

struct Matrix {
    short* matrix;
    size_t nrows;
    size_t ncols;
};

long long add(long long number1, long long number2) { return number1 + number2; }
long long multiply(long long number1, long long number2) { return number1 * number2; }

long long matrix_operation_on_column(const struct Matrix matrix,
                                     const size_t column,
                                     long long (*operator)(long long, long long),
                                     const long long initial_value) {
    long long result = initial_value;
    for (size_t i = 0; i < matrix.nrows; i++) {
        result = operator(result, (long long)matrix.matrix[i * matrix.ncols + column]);
    }
    return result;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    struct Matrix operands_matrix = {.matrix = NULL, .nrows = 4, .ncols = 1000};
    operands_matrix.matrix = calloc(operands_matrix.nrows * operands_matrix.ncols,
                                    sizeof *(operands_matrix.matrix));
    const size_t INPUT_LINE_LEN = 3750;

    /* We get the operands from the input. */
    for (size_t lines_parsed = 0; true; lines_parsed++) {
        if (lines_parsed == operands_matrix.nrows) break;

        char* const line = malloc((INPUT_LINE_LEN + 1) * sizeof *line);
        fgets(line, INPUT_LINE_LEN, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        char* num_parse_end = line;
        for (size_t i = 0; i < operands_matrix.ncols; i++) {
            operands_matrix.matrix[lines_parsed * operands_matrix.ncols + i] =
                strtol(num_parse_end, &num_parse_end, 10);
        }

        free(line);
    }

    /* We get the operators from the input. */
    char* const line = malloc((INPUT_LINE_LEN + 1) * sizeof *line);
    fgets(line, INPUT_LINE_LEN, input_file);

    /* We calculate the sum of columnar operations. */
    char* line_copy = line;
    long long operation_sum = 0;
    for (size_t j = 0; j < operands_matrix.ncols; j++, line_copy++) {
        const char* const operator_chars = "+*";
        line_copy = strpbrk(line_copy, operator_chars);

        const bool is_addition = line_copy[0] == '+';
        operation_sum +=
            (is_addition) ? matrix_operation_on_column(operands_matrix, j, add, 0)
                          : matrix_operation_on_column(operands_matrix, j, multiply, 1);
    }

    printf("The sum of the result of the operations on all columns is %lld\n",
           operation_sum);

    free(line);
    free(operands_matrix.matrix);
    fclose(input_file);
    return EXIT_SUCCESS;
}
