/* Day 06 - Part 2
 * Question Link: https://adventofcode.com/2025/day/6
 * Input Link: https://adventofcode.com/2025/day/6/input
 * */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Matrix;
struct NumberList;
long long add(long long number1, long long number2);
long long multiply(long long number1, long long number2);
short matrix_generate_number_from_col(const struct Matrix matrix, const size_t col);
struct NumberList matrix_generate_number_list_from_cols(const struct Matrix matrix,
                                                        const size_t start_col,
                                                        const size_t end_col);
long long number_list_reduce(const struct NumberList number_list,
                             long long (*operator)(long long, long long),
                             long long initial_value);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

struct Matrix {
    char* matrix;
    size_t nrows;
    size_t ncols;
};

struct NumberList {
    short* numbers;
    size_t size;
};

long long add(long long number1, long long number2) { return number1 + number2; }
long long multiply(long long number1, long long number2) { return number1 * number2; }

short matrix_generate_number_from_col(const struct Matrix matrix, const size_t col) {
    short number = 0;
    for (size_t row_i = 0; row_i < matrix.nrows; row_i++) {
        const char character = matrix.matrix[row_i * matrix.ncols + col];
        if (character == ' ') continue;

        number = number * 10 + (short)(character - '0');
    }
    return number;
}

struct NumberList matrix_generate_number_list_from_cols(const struct Matrix matrix,
                                                        const size_t start_col,
                                                        const size_t end_col) {
    const size_t nnumbers = (end_col - start_col) + 1;
    short* const numbers = calloc(nnumbers, sizeof *numbers);

    for (size_t col_i = start_col; col_i <= end_col; col_i++) {
        numbers[col_i - start_col] = matrix_generate_number_from_col(matrix, col_i);
    }

    return (struct NumberList){.numbers = numbers, .size = nnumbers};
}

long long number_list_reduce(const struct NumberList number_list,
                             long long (*operator)(long long, long long),
                             long long initial_value) {
    long long result = initial_value;
    for (size_t i = 0; i < number_list.size; i++) {
        result = operator(result, (long long)number_list.numbers[i]);
    }
    return result;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    struct Matrix matrix = {.matrix = NULL, .nrows = 4, .ncols = 3744};
    matrix.matrix = calloc(matrix.nrows * matrix.ncols, sizeof *(matrix.matrix));
    const size_t INPUT_LINE_LEN = 3750;

    /* We store the numbers lines. */
    for (size_t lines_parsed = 0; true; lines_parsed++) {
        if (lines_parsed == matrix.nrows) break;

        char* const line = malloc((INPUT_LINE_LEN + 1) * sizeof *line);
        fgets(line, INPUT_LINE_LEN, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        memcpy(matrix.matrix + (lines_parsed * matrix.ncols), line, matrix.ncols);
        free(line);
    }

    /* We extract the operator line. */
    /* Note: In input file, ensure that operator line has same num of lines as above. */
    char* const operator_line = malloc((INPUT_LINE_LEN + 1) * sizeof *operator_line);
    fgets(operator_line, INPUT_LINE_LEN, input_file);

    /* We calculate the numbers for each column and operate on them. */
    long long operation_sum = 0;
    for (size_t i = 0, prev_operator_i = 0; i < matrix.ncols; i++) {
        const bool is_last_i = i == matrix.ncols - 1, is_first_i = i == 0,
                   is_add_op = operator_line[i] == '+',
                   is_mul_op = operator_line[i] == '*';

        if (!is_first_i && (is_last_i || is_add_op || is_mul_op)) {
            const size_t end_col = (is_last_i) ? i : i - 2;
            const struct NumberList number_list =
                matrix_generate_number_list_from_cols(matrix, prev_operator_i, end_col);

            operation_sum += (operator_line[prev_operator_i] == '+')
                                 ? number_list_reduce(number_list, add, 0)
                                 : number_list_reduce(number_list, multiply, 1);

            prev_operator_i = i;
            free(number_list.numbers);
        }
    }

    printf("The sum of the result of the operations on all columns is %lld\n",
           operation_sum);

    free(operator_line);
    free(matrix.matrix);
    fclose(input_file);
    return EXIT_SUCCESS;
}
