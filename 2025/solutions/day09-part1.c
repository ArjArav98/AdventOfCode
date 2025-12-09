/* Day 09 - Part 1
 * Question Link: https://adventofcode.com/2025/day/9
 * Input Link: https://adventofcode.com/2025/day/9/input
 * */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Coord;

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

struct Coord {
    long row;
    long col;
};

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const size_t NCOORDS = 496;
    struct Coord* const coords = calloc(NCOORDS, sizeof *coords);

    long max_area = 0;

    for (size_t lines_parsed = 0; true; lines_parsed++) {
        const size_t INPUT_LINE_LEN = 20;
        char* const line = malloc((INPUT_LINE_LEN + 2) * sizeof *line);
        fgets(line, INPUT_LINE_LEN + 2, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        char* number_parse_end = line;
        coords[lines_parsed] =
            (struct Coord){.row = strtol(number_parse_end, &number_parse_end, 10),
                           .col = strtol(number_parse_end + 1, &number_parse_end, 10)};

        for (size_t i = 0; i < lines_parsed; i++) {
            const long area = labs((coords[i].row - coords[lines_parsed].row) + 1) *
                              labs((coords[i].col - coords[lines_parsed].col) + 1);
            max_area = (area > max_area) ? area : max_area;
        }

        free(line);
    }

    printf("The area of the biggest rectangle is %ld\n", max_area);

    free(coords);
    fclose(input_file);
    return EXIT_SUCCESS;
}
