#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

enum GuardDirection {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
};
enum GuardDirection guard_direction_after_turning_right(
    const enum GuardDirection guard_direction
);

struct Coord {
    int row;
    int col;
};
struct Coord coord_generate_next_coord_with_direction(
    const struct Coord guard_location,
    const enum GuardDirection guard_direction
);
bool coord_at_map_boundary(
    const struct Coord coord,
    const int square_map_length
);

bool map_obstacle_at_coord(
    const unsigned short* const map,
    const int square_map_length,
    struct Coord coord
);
void map_mark_coord_as_visited(
    unsigned short* const map,
    const int square_map_length,
    struct Coord guard_location
);
int map_get_visited_locations_count(
    const unsigned short* const map,
    const int square_map_length
);
void map_print(
    const unsigned short* const map,
    const int square_map_length,
    struct Coord guard_location
);


enum GuardDirection guard_direction_after_turning_right(
    const enum GuardDirection guard_direction
) {
    switch (guard_direction) {
        case NORTH: return EAST;
        case EAST: return SOUTH;
        case SOUTH: return WEST;
        // WEST
        default: return NORTH;
    }
}

struct Coord coord_generate_next_coord_with_direction(
    const struct Coord guard_location,
    const enum GuardDirection guard_direction
) {
    switch (guard_direction) {
        case NORTH:
            return (struct Coord) {
                .row=guard_location.row - 1, .col=guard_location.col
            };
        case EAST:
            return (struct Coord) {
                .row=guard_location.row, .col=guard_location.col + 1
            };
        case SOUTH:
            return (struct Coord) {
                .row=guard_location.row + 1, .col=guard_location.col
            };
        // WEST
        default:
            return (struct Coord) {
                .row=guard_location.row, .col=guard_location.col - 1
            };
    }
}

bool coord_at_map_boundary(
    const struct Coord coord,
    const int square_map_length
) {
    return (
        (coord.row == (square_map_length - 1)) ||
        (coord.col == (square_map_length - 1)) ||
        (coord.row == 0) ||
        (coord.col == 0)
    );
}

bool map_obstacle_at_coord(
    const unsigned short* const map,
    const int square_map_length,
    struct Coord coord
) {
    return map[(coord.row * square_map_length) + coord.col] == 1;
}

void map_mark_coord_as_visited(
    unsigned short* const map,
    const int square_map_length,
    struct Coord guard_location
) {
    map[(guard_location.row * square_map_length) + guard_location.col] = 2;
}

int map_get_visited_locations_count(
    const unsigned short* const map,
    const int square_map_length
) {
    int total_visited_locations = 0;

    for (int row_index=0; row_index < square_map_length; row_index++) {
        for (int col_index=0; col_index < square_map_length; col_index++) {
            if (map[(row_index * square_map_length) + col_index] == 2)
                total_visited_locations++;
        }
    }

    return total_visited_locations;
}

void map_print(
    const unsigned short* const map,
    const int square_map_length,
    struct Coord guard_location
) {
    for (int row_index=0; row_index < square_map_length; row_index++) {
        for (int col_index=0; col_index < square_map_length; col_index++) {
            if (row_index == guard_location.row && col_index == guard_location.col) {
                printf("^");
                continue;
            }
            if (map[(row_index * square_map_length) + col_index] == 0) printf(".");
            if (map[(row_index * square_map_length) + col_index] == 1) printf("#");
            if (map[(row_index * square_map_length) + col_index] == 2) printf("X");
        }
        printf("\n");
    }
}

int main(void) {
    FILE* const input_file = fopen("./input/day06.txt", "r");

    const int SQUARE_MAP_LENGTH = 130;
    unsigned short* const map = calloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH, sizeof(unsigned short));
    
    struct Coord guard_location = (struct Coord){ .row=0, .col=0 };
    enum GuardDirection guard_direction = NORTH;
    int current_map_row = 0;

    while(true) {
        const int max_line_len = SQUARE_MAP_LENGTH;
        char* const line = malloc(sizeof(char) * (max_line_len + 2));
        fgets(line, max_line_len + 2, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        }
    
        const size_t line_len = strlen(line);
        for (int index=0; index < line_len; index++) {
            if (line[index] == '#')
                map[(current_map_row * SQUARE_MAP_LENGTH) + index] = 1;
            if (line[index] == '^') {
                guard_location = (struct Coord){ .row=current_map_row, .col=index };
                map_mark_coord_as_visited(map, SQUARE_MAP_LENGTH, guard_location);
            }
        }

        current_map_row++;
        free(line);
    }

    while(true) {
        struct Coord new_coord = coord_generate_next_coord_with_direction(
            guard_location,
            guard_direction
        );

        if (map_obstacle_at_coord(map, SQUARE_MAP_LENGTH, new_coord)) {
            guard_direction = guard_direction_after_turning_right(guard_direction);
            continue;
        }

        guard_location = new_coord;
        map_mark_coord_as_visited(map, SQUARE_MAP_LENGTH, guard_location);

        if (coord_at_map_boundary(guard_location, SQUARE_MAP_LENGTH)) break;
    }

    printf(
        "Count of unique visited locations is %d\n",
        map_get_visited_locations_count(map, SQUARE_MAP_LENGTH)
    );

    free(map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
