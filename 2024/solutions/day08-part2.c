#include <math.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_utils.h"

struct Coord {
    long row;
    long col;
};

struct Coord coord_get_antinode_for_antennas(const struct Coord antenna1,
                                             const struct Coord antenna2);

bool antenna_map_coord_out_of_bounds(const size_t square_map_length,
                                     const struct Coord coord);

bool antenna_map_coord_is_antenna(const char* const antenna_map,
                                  const size_t square_map_length,
                                  const struct Coord coord);

void maps_print(const char* const antinode_map, const char* const antenna_map,
                const size_t square_map_length);

void antinode_map_fill_coord_with_antinode(char* const antinode_map,
                                           const size_t square_map_length,
                                           const struct Coord antinode);

void antinode_map_fill_antinodes_for_antenna(char* const antinode_map,
                                             const char* const antenna_map,
                                             const size_t square_map_length,
                                             const struct Coord antenna);

int antinode_map_antinode_count(const char* const antinode_map,
                                const size_t square_map_length);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

struct Coord coord_get_antinode_for_antennas(const struct Coord antenna1,
                                             const struct Coord antenna2) {
    return (struct Coord){
        .row = antenna1.row + (antenna1.row - antenna2.row),
        .col = antenna1.col + (antenna1.col - antenna2.col),
    };
}

bool antenna_map_coord_out_of_bounds(const size_t square_map_length,
                                     const struct Coord coord) {
    return (coord.row < 0) || (coord.row >= square_map_length) || (coord.col < 0) ||
           (coord.col >= square_map_length);
}

bool antenna_map_coord_is_antenna(const char* const antenna_map,
                                  const size_t square_map_length,
                                  const struct Coord coord) {
    const char curr_char = antenna_map[(coord.row * square_map_length) + coord.col];
    return ((curr_char != '.') && (curr_char != '#')) &&
           (((curr_char >= '0') && (curr_char <= '9')) ||
            ((curr_char >= 'A') && (curr_char <= 'Z')) ||
            ((curr_char >= 'a') && (curr_char <= 'z')));
}

void maps_print(const char* const antinode_map, const char* const antenna_map,
                const size_t square_map_length) {
    for (int i = 0; i < square_map_length; i++) {
        for (int j = 0; j < square_map_length; j++) {
            const size_t index = (i * square_map_length) + j;

            const bool antinode_found = antinode_map[index] == '#';
            const bool no_antenna_found =
                (antenna_map[index] == '.') || (antenna_map[index] == '#');

            printf("%c", (antinode_found && no_antenna_found) ? antinode_map[index]
                                                              : antenna_map[index]);
        }
        printf("\n");
    }
    printf("\n");
}

void antinode_map_fill_coord_with_antinode(char* const antinode_map,
                                           const size_t square_map_length,
                                           const struct Coord antinode) {
    const size_t index = (antinode.row * square_map_length) + antinode.col;
    antinode_map[index] = '#';
}

void antinode_map_fill_antinodes_for_antenna(char* const antinode_map,
                                             const char* const antenna_map,
                                             const size_t square_map_length,
                                             const struct Coord antenna) {
    for (int i = 0; i < square_map_length; i++) {
        for (int j = 0; j < square_map_length; j++) {
            if (antenna.row == i && antenna.col == j) continue;

            const char curr_char = antenna_map[(i * square_map_length) + j];
            const char antenna_char =
                antenna_map[(antenna.row * square_map_length) + antenna.col];
            const struct Coord coord = {.row = i, .col = j};

            if (antenna_map_coord_is_antenna(antenna_map, square_map_length, coord)) {
                if (curr_char != antenna_char) continue;

                struct Coord antenna_1 = antenna;
                struct Coord antenna_2 = coord;

                antinode_map_fill_coord_with_antinode(antinode_map, square_map_length,
                                                      antenna_1);
                antinode_map_fill_coord_with_antinode(antinode_map, square_map_length,
                                                      antenna_2);

                struct Coord antinode =
                    coord_get_antinode_for_antennas(antenna_1, antenna_2);

                while (true) {
                    if (antenna_map_coord_out_of_bounds(square_map_length, antinode))
                        break;

                    antinode_map_fill_coord_with_antinode(antinode_map,
                                                          square_map_length, antinode);

                    antenna_2 = antenna_1;
                    antenna_1 = antinode;
                    antinode = coord_get_antinode_for_antennas(antenna_1, antenna_2);
                }
            }
        }
    }
}

int antinode_map_antinode_count(const char* const antinode_map,
                                const size_t square_map_length) {
    int visible_antinodes = 0;
    for (int i = 0; i < square_map_length; i++) {
        for (int j = 0; j < square_map_length; j++) {
            if (antinode_map[(i * square_map_length) + j] == '#') visible_antinodes++;
        }
    }
    return visible_antinodes;
}

int main(void) {
    FILE* const input_file = fopen("./input/day08.txt", "r");

    const size_t SQUARE_MAP_LENGTH = 50, LINE_LEN = SQUARE_MAP_LENGTH + 2;
    char* const antenna_map =
        calloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH, sizeof(char));
    char* const antinode_map =
        calloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH, sizeof(char));

    for (char* map_copy = antenna_map; true; map_copy += SQUARE_MAP_LENGTH) {
        char* const line = malloc(sizeof(char) * LINE_LEN);
        fgets(line, LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        strncpy(map_copy, line, SQUARE_MAP_LENGTH);
        free(line);
    }

    for (int i = 0; i < SQUARE_MAP_LENGTH; i++) {
        for (int j = 0; j < SQUARE_MAP_LENGTH; j++) {
            const struct Coord coord = {.row = i, .col = j};

            if (antenna_map_coord_is_antenna(antenna_map, SQUARE_MAP_LENGTH, coord)) {
                const struct Coord antenna = coord;
                antinode_map_fill_antinodes_for_antenna(antinode_map, antenna_map,
                                                        SQUARE_MAP_LENGTH, antenna);
            }
        }
    }

    printf("Total number of unique antinode locations is %d\n",
           antinode_map_antinode_count(antinode_map, SQUARE_MAP_LENGTH));

    free(antinode_map);
    free(antenna_map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
