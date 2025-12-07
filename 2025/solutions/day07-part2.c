/* Day 07 - Part 2
 * Question Link: https://adventofcode.com/2025/day/7
 * Input Link: https://adventofcode.com/2025/day/7/input
 * */
#include "binary_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Room;
struct Coord;
long long coord_hash(const struct Coord coord, const struct Room room);
long long room_get_beam_path_possibilities(const struct Room room,
                                           const struct Coord coord,
                                           struct BinaryTree* const func_cache);
size_t room_index(const struct Room room, const struct Coord coord);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

const long long CACHE_NO_VALUE_RESULT = -1;

struct Room {
    size_t rows;
    size_t cols;

    char* grid;
    bool* grid_visited;
};

struct Coord {
    size_t row;
    size_t col;
};

long long coord_hash(const struct Coord coord, const struct Room room) {
    return (long long)room_index(room, coord);
}

size_t room_index(const struct Room room, const struct Coord coord) {
    return coord.row * room.cols + coord.col;
}

long long room_get_beam_path_possibilities(const struct Room room,
                                           const struct Coord coord,
                                           struct BinaryTree* const func_cache) {
    if (coord.row >= room.rows) return 1;

    /*----Cache Layer----*/
    const long long cache_result =
        binary_tree_get_key_value(func_cache, coord_hash(coord, room));
    if (cache_result != CACHE_NO_VALUE_RESULT) {
        return cache_result;
    }
    /*-------------------*/

    const struct Coord b_coord = {.row = coord.row + 1, .col = coord.col},
                       b_l_coord = {.row = coord.row + 1, .col = coord.col - 1},
                       b_r_coord = {.row = coord.row + 1, .col = coord.col + 1};

    const size_t coord_i = room_index(room, coord);
    room.grid_visited[coord_i] = true;

    long long possibilities_count = 0;
    const char coord_char = room.grid[coord_i];
    switch (coord_char) {
        case 'S':
            possibilities_count =
                room_get_beam_path_possibilities(room, b_coord, func_cache);
            break;
        case '.':
            possibilities_count =
                room_get_beam_path_possibilities(room, b_coord, func_cache);
            break;
        case '^':
            possibilities_count =
                room_get_beam_path_possibilities(room, b_l_coord, func_cache) +
                room_get_beam_path_possibilities(room, b_r_coord, func_cache);
            break;
    }

    /*----Cache Layer----*/
    binary_tree_insert(func_cache, coord_hash(coord, room), possibilities_count);
    /*-------------------*/

    return possibilities_count;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const size_t ROOM_ROWS = 142, ROOM_COLS = 141;
    const struct Room room = {
        .rows = ROOM_ROWS,
        .cols = ROOM_COLS,
        .grid = calloc(ROOM_ROWS * ROOM_COLS, sizeof room.grid),
        .grid_visited = calloc(ROOM_ROWS * ROOM_COLS, sizeof room.grid_visited)};
    const struct Coord start = {.row = 0, .col = room.cols / 2};

    for (size_t lines_parsed = 0; true; lines_parsed++) {
        char* const line = malloc((room.cols + 2) * sizeof *line);
        fgets(line, room.cols + 2, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        memcpy(room.grid + (lines_parsed * room.cols), line, room.cols);
        free(line);
    }

    struct BinaryTree* const func_cache = binary_tree_create(CACHE_NO_VALUE_RESULT);
    printf("The number of splits is %lld\n",
           room_get_beam_path_possibilities(room, start, func_cache));

    binary_tree_free(func_cache);
    free(room.grid);
    free(room.grid_visited);
    fclose(input_file);
    return EXIT_SUCCESS;
}
