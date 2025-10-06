#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

struct Coord {
    int row;
    int col;
};
struct XWordCoord {
    struct Coord coords[5];
};
bool x_word_coord_generate_from_coord(
    struct XWordCoord* const x_word_coord,
    int puzzle_nrows,
    int puzzle_ncols,
    struct Coord coord
);
bool x_word_coord_spells_x_shaped_mas(
    const struct XWordCoord x_word_coord, 
    char* const puzzle_grid,
    int puzzle_nrows,
    int puzzle_ncols
);
void x_word_coords_print(struct XWordCoord* const x_word_coord, int nx_word_coords);


bool x_word_coord_generate_from_coord(
    struct XWordCoord* const x_word_coord,
    int puzzle_nrows,
    int puzzle_ncols,
    struct Coord coord
) {
    if (
        ((coord.row + 1) < puzzle_nrows) &&
        ((coord.col + 1) < puzzle_ncols) &&
        ((coord.row - 1) >= 0) &&
        ((coord.col - 1) >= 0)
    ) {
        *x_word_coord = (struct XWordCoord){
            .coords = {
                { .row=coord.row-1, .col=coord.col-1 },
                { .row=coord.row-1, .col=coord.col+1 },
                coord,
                { .row=coord.row+1, .col=coord.col+1 },
                { .row=coord.row+1, .col=coord.col-1 },
            }
        };

        return 1;
    }

    return 0;
}

bool x_word_coord_spells_x_shaped_mas(
    const struct XWordCoord x_word_coord, 
    char* const puzzle_grid,
    int puzzle_nrows,
    int puzzle_ncols
) {
    char first_char = puzzle_grid[
        (x_word_coord.coords[0].row * puzzle_ncols) + x_word_coord.coords[0].col
    ];
    char second_char = puzzle_grid[
        (x_word_coord.coords[1].row * puzzle_ncols) + x_word_coord.coords[1].col
    ];
    char third_char = puzzle_grid[
        (x_word_coord.coords[2].row * puzzle_ncols) + x_word_coord.coords[2].col
    ];
    char fourth_char = puzzle_grid[
        (x_word_coord.coords[3].row * puzzle_ncols) + x_word_coord.coords[3].col
    ];
    char fifth_char = puzzle_grid[
        (x_word_coord.coords[4].row * puzzle_ncols) + x_word_coord.coords[4].col
    ];

    bool left_diagonal_spells_xmas = (
        ((first_char == 'M') && (third_char == 'A') && (fourth_char == 'S')) ||
        ((first_char == 'S') && (third_char == 'A') && (fourth_char == 'M'))
    );
    bool right_diagonal_spells_xmas = (
        ((second_char == 'M') && (third_char == 'A') && (fifth_char == 'S')) ||
        ((second_char == 'S') && (third_char == 'A') && (fifth_char == 'M'))
    );
    return left_diagonal_spells_xmas && right_diagonal_spells_xmas;
}

void x_word_coords_print(struct XWordCoord* const x_word_coord, int nx_word_coords) {
    for (int wc_index=0; wc_index < nx_word_coords; wc_index++) {
        for (int c_index=0; c_index < 5; c_index++) {
            printf(
                "{%d,%d} ",
                x_word_coord[wc_index].coords[c_index].row,
                x_word_coord[wc_index].coords[c_index].col
            );
        }
        printf("\n");
    }
}

int main(void) {
    const int COLUMNS = 140, ROWS = 140;
    char* const puzzle_grid = malloc(sizeof(char) * ROWS * (COLUMNS + 1));
    int total_xmas = 0;

    FILE* input_file = fopen("./input.txt", "r");

    for (int row_index=0; row_index < ROWS; row_index++) {
        char* const line = malloc(sizeof(char) * (COLUMNS + 2));
        fgets(line, COLUMNS + 2, input_file);
        memcpy(puzzle_grid + (row_index * COLUMNS), line, COLUMNS + 2);
        free(line);
    }

    for (int row_index=0; row_index<ROWS; row_index++) {
        for (int col_index=0; col_index<COLUMNS; col_index++) {
            if (puzzle_grid[(row_index * COLUMNS) + col_index] != 'A') continue;

            struct Coord coord = { .row=row_index, .col=col_index };
            struct XWordCoord* const x_word_coord = malloc(sizeof(struct XWordCoord));

            if (
                x_word_coord_generate_from_coord(x_word_coord, ROWS, COLUMNS, coord)
                == false
            ) {
                free(x_word_coord);
                continue; 
            }

            bool spells_x_shaped_mas = x_word_coord_spells_x_shaped_mas(
                *x_word_coord,
                puzzle_grid,
                ROWS,
                COLUMNS
            );
            if (spells_x_shaped_mas) total_xmas++;

            free(x_word_coord);
        }
    }

    printf("The count of X-shaped 'MAS' is %d\n", total_xmas);

    free(puzzle_grid);
    fclose(input_file);
    return EXIT_SUCCESS;
}
