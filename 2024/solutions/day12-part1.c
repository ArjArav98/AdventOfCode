#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Coord {
    unsigned long row;
    unsigned long col;
};

struct RegionStats {
    unsigned long area;
    unsigned long perimeter;
};

void garden_map_print(const char* const garden_map, const size_t square_garden_length);

unsigned long coord_map_index(const struct Coord coord,
                              const size_t square_garden_length);

bool coord_exceeds_garden_map_boundary(const struct Coord coord,
                                       const size_t square_garden_length);

struct RegionStats garden_map_calculate_region_stats(const char* const garden_map,
                                                     bool* const garden_visited_map,
                                                     const size_t square_garden_length,
                                                     const struct Coord coord);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

unsigned long coord_map_index(const struct Coord coord,
                              const size_t square_garden_length) {
    return coord.row * square_garden_length + coord.col;
}

bool coord_exceeds_garden_map_boundary(const struct Coord coord,
                                       const size_t square_garden_length) {
    return ((coord.row < 0 || coord.row >= square_garden_length) ||
            (coord.col < 0 || coord.col >= square_garden_length));
}

struct RegionStats garden_map_calculate_region_stats(const char* const garden_map,
                                                     bool* const garden_visited_map,
                                                     const size_t square_garden_length,
                                                     const struct Coord coord) {
    garden_visited_map[coord_map_index(coord, square_garden_length)] = true;

    struct RegionStats stats = {.area = 1, .perimeter = 0};
    const int coord_operations[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int i = 0; i < 4; i++) {
        const struct Coord new_coord = {.row = coord.row + coord_operations[i][0],
                                        .col = coord.col + coord_operations[i][1]};
        const bool different_region =
            garden_map[coord_map_index(coord, square_garden_length)] !=
            garden_map[coord_map_index(new_coord, square_garden_length)];
        const bool plot_visited =
            garden_visited_map[coord_map_index(new_coord, square_garden_length)];

        if (different_region ||
            coord_exceeds_garden_map_boundary(new_coord, square_garden_length)) {
            stats.perimeter += 1;
            continue;
        }
        if (plot_visited) continue;

        const struct RegionStats results = garden_map_calculate_region_stats(
            garden_map, garden_visited_map, square_garden_length, new_coord);
        stats.area += results.area;
        stats.perimeter += results.perimeter;
    }

    return stats;
}

void garden_map_print(const char* const garden_map, const size_t square_garden_length) {
    for (size_t i = 0; i < square_garden_length; i++) {
        for (size_t j = 0; j < square_garden_length; j++) {
            printf("%c", garden_map[i * square_garden_length + j]);
        }
        printf("\n");
    }
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");
    if (input_file == NULL) return EXIT_FAILURE;

    const size_t SQUARE_GARDEN_LENGTH = 10;
    char* const garden_map =
        calloc(SQUARE_GARDEN_LENGTH * SQUARE_GARDEN_LENGTH, sizeof *garden_map);
    bool* const garden_visited_map =
        calloc(SQUARE_GARDEN_LENGTH * SQUARE_GARDEN_LENGTH, sizeof *garden_visited_map);

    for (size_t rows_allocated = 0; rows_allocated < SQUARE_GARDEN_LENGTH;
         rows_allocated++) {
        const size_t MAX_LINE_LEN = SQUARE_GARDEN_LENGTH + 2;
        char* const line = malloc(MAX_LINE_LEN * sizeof *line);
        fgets(line, MAX_LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            return EXIT_FAILURE;
        }

        strncpy(garden_map + (rows_allocated * SQUARE_GARDEN_LENGTH),
                line,
                SQUARE_GARDEN_LENGTH);
        free(line);
    }

    unsigned long total_pricing = 0;

    for (size_t i = 0; i < SQUARE_GARDEN_LENGTH; i++) {
        for (size_t j = 0; j < SQUARE_GARDEN_LENGTH; j++) {
            struct Coord coord = {.row = i, .col = j};

            if (garden_visited_map[coord_map_index(coord, SQUARE_GARDEN_LENGTH)])
                continue;

            struct RegionStats stats = garden_map_calculate_region_stats(
                garden_map, garden_visited_map, SQUARE_GARDEN_LENGTH, coord);
            total_pricing += (stats.area * stats.perimeter);
        }
    }

    printf("The total pricing is %lu.\n", total_pricing);

    free(garden_visited_map);
    free(garden_map);
    fclose(input_file);
    return EXIT_SUCCESS;
}
