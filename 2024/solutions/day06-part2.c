#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coord {
    unsigned long row;
    unsigned long col;
};

enum Direction { UP = 1, DOWN = 2, RIGHT = 3, LEFT = 4 };

unsigned long coord_map_index(const struct Coord coord, const size_t square_map_length);

struct Coord coord_after_one_step(const struct Coord coord,
                                  const enum Direction direction);

bool coord_on_map_boundary(const struct Coord coord, const size_t square_map_length);

enum Direction direction_after_turning_right(enum Direction direction);

bool map_guard_visits_any_location_twice_in_same_direction(
    const char* const map,
    enum Direction* const visited_map,
    const size_t square_map_length,
    const struct Coord guard_initial);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

unsigned long coord_map_index(const struct Coord coord,
                              const size_t square_map_length) {
    return coord.row * square_map_length + coord.col;
}

struct Coord coord_after_one_step(const struct Coord coord,
                                  const enum Direction direction) {
    if (direction == UP)
        return (struct Coord){.row = coord.row - 1, .col = coord.col};
    else if (direction == DOWN)
        return (struct Coord){.row = coord.row + 1, .col = coord.col};
    else if (direction == RIGHT)
        return (struct Coord){.row = coord.row, .col = coord.col + 1};
    else
        return (struct Coord){.row = coord.row, .col = coord.col - 1};
}

bool coord_on_map_boundary(const struct Coord coord, const size_t square_map_length) {
    return ((coord.row == 0) || (coord.col == 0) ||
            (coord.row == square_map_length - 1) ||
            (coord.col == square_map_length - 1));
}

enum Direction direction_after_turning_right(enum Direction direction) {
    if (direction == UP)
        return RIGHT;
    else if (direction == RIGHT)
        return DOWN;
    else if (direction == DOWN)
        return LEFT;
    else
        return UP;
}

bool map_guard_visits_any_location_twice_in_same_direction(
    const char* const map,
    enum Direction* const visited_map,
    const size_t square_map_length,
    const struct Coord guard_initial) {
    struct Coord guard_location = guard_initial;
    enum Direction guard_direction = UP;
    while (true) {
        if (visited_map[coord_map_index(guard_location, square_map_length)] ==
            guard_direction)
            return true;

        if (coord_on_map_boundary(guard_location, square_map_length)) break;

        const struct Coord ahead =
            coord_after_one_step(guard_location, guard_direction);
        if (map[coord_map_index(ahead, square_map_length)] == '#') {
            guard_direction = direction_after_turning_right(guard_direction);
            continue;
        }

        visited_map[coord_map_index(guard_location, square_map_length)] =
            guard_direction;
        guard_location = ahead;
    }

    return false;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const size_t SQUARE_MAP_LENGTH = 130;
    char* const map = malloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH * sizeof *map);
    struct Coord initial_guard_location = {.row = 0, .col = 0};
    unsigned long infinite_loop_obstacle_locations = 0;

    for (size_t rows_allocated = 0; true; rows_allocated++) {
        const size_t LINE_LEN = SQUARE_MAP_LENGTH + 2;
        char* const line = malloc(LINE_LEN * sizeof *line);
        fgets(line, LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }

        const size_t guard_location = strcspn(line, "^");
        if (guard_location != strlen(line))
            initial_guard_location =
                (struct Coord){.row = rows_allocated, .col = guard_location};

        strncpy(map + (rows_allocated * SQUARE_MAP_LENGTH), line, SQUARE_MAP_LENGTH);
        free(line);
    }

    for (size_t i = 0; i < SQUARE_MAP_LENGTH; i++) {
        for (size_t j = 0; j < SQUARE_MAP_LENGTH; j++) {
            if (map[i * SQUARE_MAP_LENGTH + j] == '#') continue;

            enum Direction* const visited_map =
                calloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH, sizeof *visited_map);

            map[i * SQUARE_MAP_LENGTH + j] = '#';

            const bool is_inf_loop =
                map_guard_visits_any_location_twice_in_same_direction(
                    map, visited_map, SQUARE_MAP_LENGTH, initial_guard_location);
            if (is_inf_loop) infinite_loop_obstacle_locations++;

            map[i * SQUARE_MAP_LENGTH + j] = '.';

            free(visited_map);
        }
    }

    printf("The number of locations where placing an obstacle will cause an infinite "
           "loop is %lu.\n",
           infinite_loop_obstacle_locations);

    free(map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
