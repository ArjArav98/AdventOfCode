#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coord {
    unsigned long row;
    unsigned long col;
};

unsigned long coord_to_index(const struct Coord coord, const size_t map_cols);

struct Coord coord_generate_next_in_direction(const struct Coord coord, char direction);

bool coords_are_equal(const struct Coord coord1, const struct Coord coord2);

void map_fill_with_widened_line_input(char* const map,
                                      const size_t map_cols,
                                      const char* const line_input,
                                      const size_t row_num);

bool map_box_can_be_moved_in_direction(char* const map,
                                       const size_t map_cols,
                                       const struct Coord curr_half,
                                       const struct Coord origin,
                                       const char direction);

bool map_move_box_in_direction(char* const map,
                               const size_t map_cols,
                               const struct Coord curr_half,
                               const struct Coord origin,
                               const char direction);

struct Coord
map_move_robot_according_to_line_instructions(char* const map,
                                              const size_t map_cols,
                                              const char* const instructions,
                                              const struct Coord robot_location);

unsigned long map_calculate_sum_of_box_gps(const char* const map,
                                           const size_t map_rows,
                                           const size_t map_cols);

void map_print(const char* const map,
               const size_t map_rows,
               const size_t map_cols,
               const struct Coord robot_location);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

unsigned long coord_to_index(const struct Coord coord, const size_t map_cols) {
    return coord.row * map_cols + coord.col;
}

struct Coord coord_generate_next_in_direction(const struct Coord coord,
                                              char direction) {
    if (direction == '^')
        return (struct Coord){.row = coord.row - 1, .col = coord.col};
    else if (direction == '>')
        return (struct Coord){.row = coord.row, .col = coord.col + 1};
    else if (direction == 'v')
        return (struct Coord){.row = coord.row + 1, .col = coord.col};
    else
        return (struct Coord){.row = coord.row, .col = coord.col - 1};
}

bool coords_are_equal(const struct Coord coord1, const struct Coord coord2) {
    return coord1.row == coord2.row && coord1.col == coord2.col;
}

void map_fill_with_widened_line_input(char* const map,
                                      const size_t map_cols,
                                      const char* const line_input,
                                      const size_t row_num) {
    const size_t line_input_len = strlen(line_input);

    for (size_t i = 0; i < line_input_len; i++) {
        if (line_input[i] == '.') {
            map[(row_num * map_cols) + (i * 2)] = '.';
            map[(row_num * map_cols) + (i * 2) + 1] = '.';
        } else if (line_input[i] == 'O') {
            map[(row_num * map_cols) + (i * 2)] = '[';
            map[(row_num * map_cols) + (i * 2) + 1] = ']';
        } else if (line_input[i] == '#') {
            map[(row_num * map_cols) + (i * 2)] = '#';
            map[(row_num * map_cols) + (i * 2) + 1] = '#';
        }
    }
}

bool map_box_can_be_moved_in_direction(char* const map,
                                       const size_t map_cols,
                                       const struct Coord curr_half,
                                       const struct Coord origin,
                                       const char direction) {
    const struct Coord other_half = {
        .row = curr_half.row,
        .col = (map[coord_to_index(curr_half, map_cols)] == '[') ? curr_half.col + 1
                                                                 : curr_half.col - 1};
    const struct Coord next = coord_generate_next_in_direction(curr_half, direction);
    const char next_char = map[coord_to_index(next, map_cols)];

    if (next_char == '#') return false;
    if (next_char == '[' || next_char == ']') {
        const bool next_movable = map_box_can_be_moved_in_direction(
            map, map_cols, next, curr_half, direction);
        if (!next_movable) return false;
    }

    const bool other_half_already_checked =
        coords_are_equal(other_half, origin) || coords_are_equal(other_half, next);
    if (!other_half_already_checked) {
        const bool other_half_movable = map_box_can_be_moved_in_direction(
            map, map_cols, other_half, curr_half, direction);
        if (!other_half_movable) return false;
    }

    return true;
}

bool map_move_box_in_direction(char* const map,
                               const size_t map_cols,
                               const struct Coord curr_half,
                               const struct Coord origin,
                               const char direction) {
    if (!map_box_can_be_moved_in_direction(map, map_cols, curr_half, origin, direction))
        return false;

    const struct Coord other_half = {
        .row = curr_half.row,
        .col = (map[coord_to_index(curr_half, map_cols)] == '[') ? curr_half.col + 1
                                                                 : curr_half.col - 1};

    const struct Coord next = coord_generate_next_in_direction(curr_half, direction);
    const char next_char = map[coord_to_index(next, map_cols)];

    if (next_char == '#')
        return false;
    else if (next_char == '[' || next_char == ']') {
        const bool next_moved =
            map_move_box_in_direction(map, map_cols, next, curr_half, direction);
        if (!next_moved) return false;
    }

    const bool other_half_already_checked =
        coords_are_equal(other_half, origin) || coords_are_equal(other_half, next);
    if (!other_half_already_checked) {
        const bool other_half_movable =
            map_move_box_in_direction(map, map_cols, other_half, curr_half, direction);
        if (!other_half_movable) return false;
    }

    map[coord_to_index(next, map_cols)] = map[coord_to_index(curr_half, map_cols)];
    map[coord_to_index(curr_half, map_cols)] = '.';

    return true;
}

struct Coord
map_move_robot_according_to_line_instructions(char* const map,
                                              const size_t map_cols,
                                              const char* const instructions,
                                              const struct Coord robot_location) {
    const size_t ninstructions = strlen(instructions);
    struct Coord curr_location = robot_location;

    for (size_t i = 0; i < ninstructions - 1; i++) {
        const struct Coord next_coord =
            coord_generate_next_in_direction(curr_location, instructions[i]);
        const char next_coord_char = map[coord_to_index(next_coord, map_cols)];

        if (next_coord_char == '#')
            continue;
        else if (next_coord_char == ']' || next_coord_char == '[') {
            const bool box_moved = map_move_box_in_direction(
                map, map_cols, next_coord, curr_location, instructions[i]);
            if (!box_moved) continue;
        }

        curr_location = next_coord;
    }

    return curr_location;
}

unsigned long map_calculate_sum_of_box_gps(const char* const map,
                                           const size_t map_rows,
                                           const size_t map_cols) {
    unsigned long gps_sum = 0;

    for (size_t i = 0; i < map_rows; i++) {
        for (size_t j = 0; j < map_cols; j++) {
            const unsigned long gps = i * 100 + j;
            if (map[i * map_cols + j] == '[') gps_sum += gps;
        }
    }

    return gps_sum;
}

void map_print(const char* const map,
               const size_t map_rows,
               const size_t map_cols,
               const struct Coord robot_location) {
    for (size_t i = 0; i < map_rows; i++) {
        for (size_t j = 0; j < map_cols; j++) {
            if (robot_location.row == i && robot_location.col == j) {
                printf("@");
                continue;
            }
            printf("%c", map[i * map_cols + j]);
        }
        printf("\n");
    }
}

int main(void) {
    FILE* input_file = fopen("./input.txt", "r");

    const size_t MAP_COLS = 100, MAP_ROWS = MAP_COLS / 2, INPUT_MAP_COLS = MAP_COLS / 2,
                 MAP_LINE_LEN = INPUT_MAP_COLS + 2, ROBOT_MOV_LINE_LEN = 1010;
    char* const map = calloc(MAP_COLS * MAP_ROWS, sizeof *map);
    struct Coord robot_location = {.row = 0, .col = 0};

    for (size_t map_rows_ingested = 0; true; map_rows_ingested++) {
        char* const line = malloc(MAP_LINE_LEN * sizeof *line);
        fgets(line, MAP_LINE_LEN, input_file);

        if (strlen(line) == 1) {
            free(line);
            break;
        }

        const size_t robot_location_in_line = strcspn(line, "@");
        if (robot_location_in_line != strlen(line)) {
            robot_location = (struct Coord){.row = map_rows_ingested,
                                            .col = robot_location_in_line * 2};
            line[robot_location_in_line] = '.';
        }

        map_fill_with_widened_line_input(map, MAP_COLS, line, map_rows_ingested);
        free(line);
    }

    while (true) {
        char* const line = malloc(ROBOT_MOV_LINE_LEN * sizeof *line);
        fgets(line, ROBOT_MOV_LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        robot_location = map_move_robot_according_to_line_instructions(
            map, MAP_COLS, line, robot_location);

        free(line);
    }

    map_print(map, MAP_ROWS, MAP_COLS, robot_location);
    printf("Sum of all box GPS coordinates is %lu.\n",
           map_calculate_sum_of_box_gps(map, MAP_ROWS, MAP_COLS));

    free(map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
