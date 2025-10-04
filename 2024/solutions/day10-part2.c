#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coord {
    size_t row;
    size_t col;
};

size_t coord_map_index(const struct Coord coord, const size_t square_map_length);

void visit_count_map_fill_9s_visited_count(unsigned long* const visit_count_map,
                                           const short* const map,
                                           const size_t square_map_length,
                                           const struct Coord trailhead);
unsigned long
visit_count_map_count_9s_visits(const unsigned long* const visit_count_map,
                                const size_t square_map_length);

void map_print(const short* const map, const size_t square_map_length);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

size_t coord_map_index(const struct Coord coord, const size_t square_map_length) {
    return (coord.row * square_map_length) + coord.col;
}

void visit_count_map_fill_9s_visited_count(unsigned long* const visit_count_map,
                                           const short* const map,
                                           const size_t square_map_length,
                                           const struct Coord trailhead) {
    const size_t trailhead_i = coord_map_index(trailhead, square_map_length);
    if (map[trailhead_i] == 9) {
        visit_count_map[trailhead_i]++;
        return;
    }

    struct Coord coords[4];
    int coords_to_check = 0;

    if (trailhead.row != 0) {
        coords[coords_to_check] =
            (struct Coord){.row = trailhead.row - 1, .col = trailhead.col};
        coords_to_check++;
    }
    if (trailhead.row != square_map_length) {
        coords[coords_to_check] =
            (struct Coord){.row = trailhead.row + 1, .col = trailhead.col};
        coords_to_check++;
    }
    if (trailhead.col != square_map_length) {
        coords[coords_to_check] =
            (struct Coord){.row = trailhead.row, .col = trailhead.col + 1};
        coords_to_check++;
    }
    if (trailhead.col != 0) {
        coords[coords_to_check] =
            (struct Coord){.row = trailhead.row, .col = trailhead.col - 1};
        coords_to_check++;
    }

    for (int i = 0; i < coords_to_check; i++) {
        const size_t new_coord_i = coord_map_index(coords[i], square_map_length);
        if (map[new_coord_i] == (map[trailhead_i] + 1))
            visit_count_map_fill_9s_visited_count(
                visit_count_map, map, square_map_length, coords[i]);
    }
}

unsigned long
visit_count_map_count_9s_visits(const unsigned long* const visit_count_map,
                                const size_t square_map_length) {
    unsigned long visit_count = 0;
    for (size_t i = 0; i < square_map_length; i++) {
        for (size_t j = 0; j < square_map_length; j++) {
            visit_count += visit_count_map[(i * square_map_length) + j];
        }
    }
    return visit_count;
}

int main(void) {
    FILE* input_file = fopen("./input/day10.txt", "r");

    const size_t SQUARE_MAP_LENGTH = 45;
    short* const map = calloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH, sizeof(short));
    unsigned long total_ratings = 0;

    for (size_t rows_extracted = 0; true; rows_extracted++) {
        const size_t MAX_LINE_SIZE = SQUARE_MAP_LENGTH + 2;
        char* const line = malloc(sizeof(char) * MAX_LINE_SIZE);
        fgets(line, MAX_LINE_SIZE, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        for (size_t i = 0; i < SQUARE_MAP_LENGTH; i++)
            map[(rows_extracted * SQUARE_MAP_LENGTH) + i] = line[i] - '0';

        free(line);
    }

    for (size_t i = 0; i < SQUARE_MAP_LENGTH; i++) {
        for (size_t j = 0; j < SQUARE_MAP_LENGTH; j++) {
            struct Coord coord = {.row = i, .col = j};
            if (map[coord_map_index(coord, SQUARE_MAP_LENGTH)] != 0) continue;

            unsigned long* const visit_count_map =
                calloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH, sizeof(unsigned long));

            visit_count_map_fill_9s_visited_count(
                visit_count_map, map, SQUARE_MAP_LENGTH, coord);
            total_ratings +=
                visit_count_map_count_9s_visits(visit_count_map, SQUARE_MAP_LENGTH);

            free(visit_count_map);
        }
    }

    printf("The total ratings is %lu.\n", total_ratings);

    free(map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
