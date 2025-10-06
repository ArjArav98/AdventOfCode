#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

struct Coord {
    int row;
    int col;
};
struct WordCoord {
    struct Coord coords[4];
};
int word_coords_generate_from_coord(
    struct WordCoord* const word_coords,
    int puzzle_nrows,
    int puzzle_ncols,
    struct Coord coord
);
bool word_coord_spells_xmas(
    const struct WordCoord word_coord, 
    char* const puzzle_grid,
    int puzzle_nrows,
    int puzzle_ncols
);
void word_coords_print(struct WordCoord* const word_coords, int nword_coords);


int word_coords_generate_from_coord(
    struct WordCoord* const word_coords,
    int puzzle_nrows,
    int puzzle_ncols,
    struct Coord coord
) {
    int word_coord_index = 0;
    // VERTICAL DOWN
    if ((coord.row + 3) < puzzle_nrows) {
        word_coords[word_coord_index] = (struct WordCoord){
            .coords = {
                coord,
                { .row=coord.row+1, .col=coord.col },
                { .row=coord.row+2, .col=coord.col },
                { .row=coord.row+3, .col=coord.col },
            }
        };
        word_coord_index++;
    }
    // VERTICAL UP
    if ((coord.row - 3) >= 0) {
        word_coords[word_coord_index] = (struct WordCoord){
            .coords = {
                coord,
                { .row=coord.row-1, .col=coord.col },
                { .row=coord.row-2, .col=coord.col },
                { .row=coord.row-3, .col=coord.col },
            }
        };
        word_coord_index++;
    }
    // HORIZONTAL RIGHT
    if ((coord.col + 3) < puzzle_ncols) {
        word_coords[word_coord_index] = (struct WordCoord){
            .coords = {
                coord,
                { .row=coord.row, .col=coord.col+1 },
                { .row=coord.row, .col=coord.col+2 },
                { .row=coord.row, .col=coord.col+3 },
            }
        };
        word_coord_index++;
    }
    // HORIZONTAL LEFT
    if ((coord.col - 3) >= 0) {
        word_coords[word_coord_index] = (struct WordCoord){
            .coords = {
                coord,
                { .row=coord.row, .col=coord.col-1 },
                { .row=coord.row, .col=coord.col-2 },
                { .row=coord.row, .col=coord.col-3 },
            }
        };
        word_coord_index++;
    }
    // DIAGONAL TOP RIGHT
    if (((coord.row - 3) >= 0) && ((coord.col + 3) < puzzle_ncols)) {
        word_coords[word_coord_index] = (struct WordCoord){
            .coords = {
                coord,
                { .row=coord.row-1, .col=coord.col+1 },
                { .row=coord.row-2, .col=coord.col+2 },
                { .row=coord.row-3, .col=coord.col+3 },
            }
        };
        word_coord_index++;
    }
    // DIAGONAL BOTTOM RIGHT
    if (((coord.row + 3) < puzzle_nrows) && ((coord.col + 3) < puzzle_ncols)) {
        word_coords[word_coord_index] = (struct WordCoord){
            .coords = {
                coord,
                { .row=coord.row+1, .col=coord.col+1 },
                { .row=coord.row+2, .col=coord.col+2 },
                { .row=coord.row+3, .col=coord.col+3 },
            }
        };
        word_coord_index++;
    }
    // DIAGONAL BOTTOM LEFT
    if (((coord.row + 3) < puzzle_nrows) && ((coord.col - 3) >= 0)) {
        word_coords[word_coord_index] = (struct WordCoord){
            .coords = {
                coord,
                { .row=coord.row+1, .col=coord.col-1 },
                { .row=coord.row+2, .col=coord.col-2 },
                { .row=coord.row+3, .col=coord.col-3 },
            }
        };
        word_coord_index++;
    }
    // DIAGONAL TOP LEFT
    if (((coord.row - 3) >= 0) && ((coord.col - 3) >= 0)) {
        word_coords[word_coord_index] = (struct WordCoord){
            .coords = {
                coord,
                { .row=coord.row-1, .col=coord.col-1 },
                { .row=coord.row-2, .col=coord.col-2 },
                { .row=coord.row-3, .col=coord.col-3 },
            }
        };
        word_coord_index++;
    }

    return word_coord_index;
}

bool word_coord_spells_xmas(
    const struct WordCoord word_coord, 
    char* const puzzle_grid,
    int puzzle_nrows,
    int puzzle_ncols
) {
    char first_char = puzzle_grid[
        (word_coord.coords[0].row * puzzle_ncols) + word_coord.coords[0].col
    ];
    char second_char = puzzle_grid[
        (word_coord.coords[1].row * puzzle_ncols) + word_coord.coords[1].col
    ];
    char third_char = puzzle_grid[
        (word_coord.coords[2].row * puzzle_ncols) + word_coord.coords[2].col
    ];
    char fourth_char = puzzle_grid[
        (word_coord.coords[3].row * puzzle_ncols) + word_coord.coords[3].col
    ];
    return
        (first_char == 'X') &&
        (second_char == 'M') &&
        (third_char == 'A') &&
        (fourth_char == 'S');
}

void word_coords_print(struct WordCoord* const word_coords, int nword_coords) {
    for (int wc_index=0; wc_index < nword_coords; wc_index++) {
        for (int c_index=0; c_index < 4; c_index++) {
            printf(
                "{%d,%d} ",
                word_coords[wc_index].coords[c_index].row,
                word_coords[wc_index].coords[c_index].col
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
            if (puzzle_grid[(row_index * COLUMNS) + col_index] != 'X') continue;

            struct Coord coord = { .row=row_index, .col=col_index };
            struct WordCoord* const word_coords = malloc(sizeof(struct WordCoord) * 8);

            int word_coords_count = word_coords_generate_from_coord(
                word_coords,
                ROWS, COLUMNS,
                coord
            );

            for (int index=0; index < word_coords_count; index++) {
                bool spells_xmas = word_coord_spells_xmas(
                    word_coords[index],
                    puzzle_grid,
                    ROWS,
                    COLUMNS
                );
                if (spells_xmas) total_xmas++;
            }

            free(word_coords);
        }
    }

    printf("The count of 'XMAS' is %d\n", total_xmas);

    free(puzzle_grid);
    fclose(input_file);
    return EXIT_SUCCESS;
}
