/* Day 04 - Part 1
 * Question Link: https://adventofcode.com/2025/day/4
 * Input Link: https://adventofcode.com/2025/day/4/input
 * */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coord;
size_t coord_index(const struct Coord coord, const size_t square_map_length);
bool map_coord_exceeds_boundary(const struct Coord coord,
                                const size_t square_map_length);
short map_element_neighbour_count(const char* const map,
                                  const struct Coord coord,
                                  const size_t square_map_length);
short map_elements_with_less_than_four_neighbours(const char* const map,
                                                  const size_t square_map_length);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

struct Coord {
    unsigned short row;
    unsigned short col;
};

size_t coord_index(const struct Coord coord, const size_t square_map_length) {
    return coord.row * square_map_length + coord.col;
}

bool map_coord_exceeds_boundary(const struct Coord coord,
                                const size_t square_map_length) {
    return (coord.row >= square_map_length) || (coord.col >= square_map_length);
}

short map_element_neighbour_count(const char* const map,
                                  const struct Coord coord,
                                  const size_t square_map_length) {
    const struct Coord operands[8] = {
        (struct Coord){.row = 0, .col = 1},
        (struct Coord){.row = 0, .col = -1},
        (struct Coord){.row = 1, .col = 1},
        (struct Coord){.row = 1, .col = -1},
        (struct Coord){.row = 1, .col = 0},
        (struct Coord){.row = -1, .col = 1},
        (struct Coord){.row = -1, .col = -1},
        (struct Coord){.row = -1, .col = 0},
    };
    short element_neighbours = 0;

    for (size_t i = 0; i < 8; i++) {
        const struct Coord new_coord = {.row = coord.row + operands[i].row,
                                        .col = coord.col + operands[i].col};
        if (map_coord_exceeds_boundary(new_coord, square_map_length)) continue;
        else if (map[coord_index(new_coord, square_map_length)] == '@')
            element_neighbours++;
    }

    return element_neighbours;
}

short map_elements_with_less_than_four_neighbours(const char* const map,
                                                  const size_t square_map_length) {
    short count = 0;

    for (size_t i = 0; i < square_map_length; i++) {
        for (size_t j = 0; j < square_map_length; j++) {
            const struct Coord coord = {.row = i, .col = j};
            if (map[coord_index(coord, square_map_length)] == '.') continue;
            else if (map_element_neighbour_count(map, coord, square_map_length) < 4)
                count++;
        }
    }

    return count;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const size_t SQUARE_MAP_LENGTH = 135;
    char* const map = calloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH, sizeof *map);

    for (size_t i = 0; true; i++) {
        const size_t LINE_LENGTH = SQUARE_MAP_LENGTH + 2;
        char* const line = malloc(LINE_LENGTH * sizeof *line);

        fgets(line, LINE_LENGTH, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        strncpy(map + SQUARE_MAP_LENGTH * i, line, SQUARE_MAP_LENGTH);
        free(line);
    }

    printf("The number of elements with <4 neighbours is %hd.\n",
           map_elements_with_less_than_four_neighbours(map, SQUARE_MAP_LENGTH));

    free(map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
