#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coord {
    unsigned long row;
    unsigned long col;
};

unsigned long coord_to_index(const struct Coord coord, const size_t square_map_length);
struct Coord coord_generate_next_in_direction(const struct Coord coord, char direction);
bool map_move_box_in_direction(char* const map,
                               const size_t square_map_length,
                               const struct Coord box_coord,
                               char direction);
struct Coord
map_move_robot_according_to_line_instructions(char* const map,
                                              const size_t square_map_length,
                                              const char* const instructions,
                                              const struct Coord robot_location);
unsigned long map_calculate_sum_of_box_gps(const char* const map,
                                           const size_t square_map_length);
void map_print(const char* const map,
               const size_t square_map_length,
               const struct Coord robot_location);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

unsigned long coord_to_index(const struct Coord coord, const size_t square_map_length) {
    return coord.row * square_map_length + coord.col;
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

bool map_move_box_in_direction(char* const map,
                               const size_t square_map_length,
                               const struct Coord box_coord,
                               char direction) {
    const struct Coord next_coord =
        coord_generate_next_in_direction(box_coord, direction);
    const char next_coord_char = map[coord_to_index(next_coord, square_map_length)];

    if (next_coord_char == '#')
        return false;
    else if (next_coord_char == 'O') {
        const bool box_moved =
            map_move_box_in_direction(map, square_map_length, next_coord, direction);
        if (!box_moved) return false;
    };

    map[coord_to_index(next_coord, square_map_length)] = 'O';
    map[coord_to_index(box_coord, square_map_length)] = '.';

    return true;
}

struct Coord
map_move_robot_according_to_line_instructions(char* const map,
                                              const size_t square_map_length,
                                              const char* const instructions,
                                              const struct Coord robot_location) {
    const size_t ninstructions = strlen(instructions);
    struct Coord curr_location = robot_location;

    for (size_t i = 0; i < ninstructions - 1; i++) {
        const struct Coord next_coord =
            coord_generate_next_in_direction(curr_location, instructions[i]);
        const char next_coord_char = map[coord_to_index(next_coord, square_map_length)];

        if (next_coord_char == '#')
            continue;
        else if (next_coord_char == 'O') {
            const bool box_moved = map_move_box_in_direction(
                map, square_map_length, next_coord, instructions[i]);
            if (!box_moved) continue;
        }

        curr_location = next_coord;
    }

    return curr_location;
}

unsigned long map_calculate_sum_of_box_gps(const char* const map,
                                           const size_t square_map_length) {
    unsigned long gps_sum = 0;

    for (size_t i = 0; i < square_map_length; i++) {
        for (size_t j = 0; j < square_map_length; j++) {
            const unsigned long gps = i * 100 + j;
            if (map[i * square_map_length + j] == 'O') gps_sum += gps;
        }
    }

    return gps_sum;
}

void map_print(const char* const map,
               const size_t square_map_length,
               const struct Coord robot_location) {
    for (size_t i = 0; i < square_map_length; i++) {
        for (size_t j = 0; j < square_map_length; j++) {
            if (robot_location.row == i && robot_location.col == j) {
                printf("@");
                continue;
            }
            printf("%c", map[i * square_map_length + j]);
        }
        printf("\n");
    }
}

int main(void) {
    FILE* input_file = fopen("./input.txt", "r");

    const size_t SQUARE_MAP_LENGTH = 50, MAP_LINE_LEN = SQUARE_MAP_LENGTH + 2,
                 ROBOT_MOV_LINE_LEN = 1010;
    char* const map = calloc(SQUARE_MAP_LENGTH * SQUARE_MAP_LENGTH, sizeof *map);
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
            robot_location =
                (struct Coord){.row = map_rows_ingested, .col = robot_location_in_line};
            line[robot_location_in_line] = '.';
        }

        strncpy(map + (map_rows_ingested * SQUARE_MAP_LENGTH), line, SQUARE_MAP_LENGTH);
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
            map, SQUARE_MAP_LENGTH, line, robot_location);

        free(line);
    }

    map_print(map, SQUARE_MAP_LENGTH, robot_location);
    printf("Sum of all box GPS coordinates is %lu.\n",
           map_calculate_sum_of_box_gps(map, SQUARE_MAP_LENGTH));

    free(map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
