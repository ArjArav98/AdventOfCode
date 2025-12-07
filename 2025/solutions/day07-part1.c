/* Day 07 - Part 1
 * Question Link: https://adventofcode.com/2025/day/7
 * Input Link: https://adventofcode.com/2025/day/7/input
 * */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Room;
struct Coord;
short room_get_beam_unique_split_count(const struct Room room,
                                       const struct Coord coord);
size_t room_index(const struct Room room, const struct Coord coord);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

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

size_t room_index(const struct Room room, const struct Coord coord) {
    return coord.row * room.cols + coord.col;
}

short room_get_beam_unique_split_count(const struct Room room,
                                       const struct Coord coord) {
    if (coord.row >= room.rows) return 0;

    const struct Coord b_coord = {.row = coord.row + 1, .col = coord.col},
                       b_l_coord = {.row = coord.row + 1, .col = coord.col - 1},
                       b_r_coord = {.row = coord.row + 1, .col = coord.col + 1};

    const bool b_l_coord_visited = room.grid_visited[room_index(room, b_l_coord)],
               b_r_coord_visited = room.grid_visited[room_index(room, b_r_coord)];

    const size_t coord_i = room_index(room, coord);
    room.grid_visited[coord_i] = true;

    const char coord_char = room.grid[coord_i];
    switch (coord_char) {
        case 'S':
            return room_get_beam_unique_split_count(room, b_coord);
        case '.':
            return room_get_beam_unique_split_count(room, b_coord);
        case '^':
            if (b_l_coord_visited && b_r_coord_visited) {
                return 0;
            } else if (b_l_coord_visited && !b_r_coord_visited) {
                return 1 + room_get_beam_unique_split_count(room, b_r_coord);
            } else if (!b_l_coord_visited && b_r_coord_visited) {
                return 1 + room_get_beam_unique_split_count(room, b_l_coord);
            } else {
                return 1 + room_get_beam_unique_split_count(room, b_r_coord) +
                       room_get_beam_unique_split_count(room, b_l_coord);
            }
    }

    return 0;
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

    printf("The number of splits is %hd\n",
           room_get_beam_unique_split_count(room, start));

    free(room.grid);
    free(room.grid_visited);
    fclose(input_file);
    return EXIT_SUCCESS;
}
