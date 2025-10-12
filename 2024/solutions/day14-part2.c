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
struct Coord robot_calculate_position_after_n_seconds(const struct Robot robot,
                                                      const short seconds,
                                                      const size_t area_rows,
                                                      const size_t area_cols);
void area_print(const short* const area,
                const size_t area_rows,
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

struct Coord robot_calculate_position_after_n_seconds(const struct Robot robot,
                                                      const short seconds,
                                                      const size_t area_rows,
                                                      const size_t area_cols) {
    const long row =
        ((robot.velocity_change.row * (long)seconds) + robot.initial_position.row) %
        (long)area_rows;
    const long col =
        ((robot.velocity_change.col * (long)seconds) + robot.initial_position.col) %
        (long)area_cols;

    return (struct Coord){
        .row = (row < 0) ? area_rows + row : row,
        .col = (col < 0) ? area_cols + col : col,
    };
}

void area_print(const short* const area,
                const size_t area_rows,
                const size_t area_cols) {
    for (size_t i = 0; i < area_rows; i++) {
        for (size_t j = 0; j < area_cols; j++) {
            printf("%hd", area[i * area_cols + j]);
        }
        printf("\n");
    }
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const size_t AREA_ROWS = 103, AREA_COLS = 101;
    const size_t MAX_ROBOTS = 500;
    struct Robot* const robots = calloc(MAX_ROBOTS, sizeof *robots);
    size_t nrobots = 0;

    while (true) {
        const size_t LINE_LEN = 50;
        char* const line = malloc(LINE_LEN * sizeof *line);
        fgets(line, LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        robots[nrobots] = robot_parse_from_line(line);

        free(line);
        nrobots++;
    }

    for (size_t second_i = 0; true; second_i++) {
        short* const area = calloc(AREA_ROWS * AREA_COLS, sizeof *area);
        bool multiple_robots_in_one_location = false;

        for (size_t robot_i = 0; robot_i < nrobots; robot_i++) {
            const struct Coord curr_pos = robot_calculate_position_after_n_seconds(
                robots[robot_i], second_i, AREA_ROWS, AREA_COLS);
            area[coord_to_index(curr_pos, AREA_COLS)]++;

            if (area[coord_to_index(curr_pos, AREA_COLS)] > 1) {
                multiple_robots_in_one_location = true;
                break;
            }
        }

        /* Assumption: If the robots are arranging themselves into a position,
         * then no robot would overlap with another. We can discard all
         * formations where there is more than one robot in a single place. */
        if (multiple_robots_in_one_location) {
            free(area);
            continue;
        }

        area_print(area, AREA_ROWS, AREA_COLS);
        printf("\nThe minimum number of seconds to display the easter egg is %lu.\n",
               second_i);

        free(area);
        break;
    }

    free(robots);
    fclose(input_file);
    return EXIT_SUCCESS;
}
