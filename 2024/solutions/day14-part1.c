#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coord {
    long row;
    long col;
};

struct Robot {
    struct Coord initial_position;
    struct Coord velocity_change;
};

size_t coord_to_index(const struct Coord coord, const size_t area_cols);
struct Robot robot_parse_from_line(const char* const line);
struct Coord robot_calculate_position_after_100_seconds(const struct Robot robot,
                                                        const size_t area_rows,
                                                        const size_t area_cols);
unsigned long area_quadrant_robot_count(const short* const area,
                                        const size_t area_rows_start,
                                        const size_t area_rows_end,
                                        const size_t area_cols_start,
                                        const size_t area_cols_end,
                                        const size_t area_cols);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

size_t coord_to_index(const struct Coord coord, const size_t area_cols) {
    return (coord.row * area_cols) + coord.col;
}

struct Robot robot_parse_from_line(const char* const line) {
    char* number_end = NULL;

    const long number1 = strtol(strpbrk(line, "-+0123456789"), &number_end, 10);
    const long number2 = strtol(strpbrk(number_end, "-+0123456789"), &number_end, 10);
    const long number3 = strtol(strpbrk(number_end, "-+0123456789"), &number_end, 10);
    const long number4 = strtol(strpbrk(number_end, "-+0123456789"), &number_end, 10);

    return (struct Robot){
        .initial_position = (struct Coord){.col = number1, .row = number2},
        .velocity_change = (struct Coord){.col = number3, .row = number4}};
}

struct Coord robot_calculate_position_after_100_seconds(const struct Robot robot,
                                                        const size_t area_rows,
                                                        const size_t area_cols) {
    const long row = ((robot.velocity_change.row * 100) + robot.initial_position.row) %
                     (long)area_rows;
    const long col = ((robot.velocity_change.col * 100) + robot.initial_position.col) %
                     (long)area_cols;

    return (struct Coord){
        .row = (row < 0) ? area_rows + row : row,
        .col = (col < 0) ? area_cols + col : col,
    };
}

unsigned long area_quadrant_robot_count(const short* const area,
                                        const size_t area_rows_start,
                                        const size_t area_rows_end,
                                        const size_t area_cols_start,
                                        const size_t area_cols_end,
                                        const size_t area_cols) {
    unsigned long robot_count = 0;
    for (size_t i = area_rows_start; i < area_rows_end; i++) {
        for (size_t j = area_cols_start; j < area_cols_end; j++) {
            robot_count += area[i * area_cols + j];
        }
    }

    return robot_count;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const size_t AREA_ROWS = 103, AREA_COLS = 101;
    short* const area = calloc(AREA_ROWS * AREA_COLS, sizeof *area);

    while (true) {
        const size_t LINE_LEN = 50;
        char* const line = malloc(LINE_LEN * sizeof *line);
        fgets(line, LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        const struct Robot robot = robot_parse_from_line(line);
        const struct Coord calculated_position =
            robot_calculate_position_after_100_seconds(robot, AREA_ROWS, AREA_COLS);
        area[coord_to_index(calculated_position, AREA_COLS)]++;

        free(line);
    }

    long safety_factor =
        area_quadrant_robot_count(area, 0, AREA_ROWS / 2, 0, AREA_COLS / 2, AREA_COLS);
    safety_factor *= area_quadrant_robot_count(
        area, 0, AREA_ROWS / 2, (AREA_COLS / 2) + 1, AREA_COLS, AREA_COLS);
    safety_factor *= area_quadrant_robot_count(
        area, (AREA_ROWS / 2) + 1, AREA_ROWS, 0, AREA_COLS / 2, AREA_COLS);
    safety_factor *= area_quadrant_robot_count(area,
                                               (AREA_ROWS / 2) + 1,
                                               AREA_ROWS,
                                               (AREA_COLS / 2) + 1,
                                               AREA_COLS,
                                               AREA_COLS);

    printf("The safety factor is %ld\n", safety_factor);

    free(area);
    fclose(input_file);
    return EXIT_SUCCESS;
}
