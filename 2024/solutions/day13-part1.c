#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned long BUTTON_A_TOKENS = 3;
const unsigned long BUTTON_B_TOKENS = 1;

struct Coord {
    unsigned long x;
    unsigned long y;
};

struct Coord coord_extract_from_line(const char* const line);
unsigned long min_tokens_to_win_prize(const struct Coord button_a,
                                      const struct Coord button_b,
                                      const struct Coord prize);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

struct Coord coord_extract_from_line(const char* const line) {
    char* end_of_first_num;
    return (struct Coord){
        .x = strtol(strpbrk(line, "0123456789"), &end_of_first_num, 10),
        .y = strtol(strpbrk(end_of_first_num, "0123456789"), NULL, 10)};
}

unsigned long min_tokens_to_win_prize(const struct Coord button_a,
                                      const struct Coord button_b,
                                      const struct Coord prize) {
    /* We are given two equations:
     *
     * (ButtonA.x * x) + (ButtonB.x * y) = Prize.x
     * (ButtonA.y * x) + (ButtonB.y * y) = Prize.y
     *
     * Here,    x is the number of times ButtonA is pressed.
     *          y is the number of times ButtonB is pressed.
     *
     * Solving for y, we get:
     *
     * y =   (ButtonA.y * Prize.x) - (ButtonA.x * Prize.y)
     *     ------------------------------------------------
     *     (ButtonA.y * ButtonB.x) - (ButtonA.x * ButtonB.y)
     *
     * Using equation 2, solving for x, we get:
     *
     * x = Prize.y - (ButtonB.y * y)
     *     -------------------------
     *            ButtonA.y
     * */
    const double button_a_x = button_a.x, button_b_x = button_b.x,
                 button_a_y = button_a.y, button_b_y = button_b.y, prize_x = prize.x,
                 prize_y = prize.y;

    const unsigned long
        b_presses =
            (unsigned long)(((button_a_y * prize_x) - (button_a_x * prize_y)) /
                            ((button_a_y * button_b_x) - (button_a_x * button_b_y))),
        a_presses = (unsigned long)((prize_y - (button_b_y * b_presses)) / button_a_y);

    const bool x_axes_match =
                   ((button_a.x * a_presses) + (button_b.x * b_presses)) == prize.x,
               y_axes_match =
                   ((button_a.y * a_presses) + (button_b.y * b_presses)) == prize.y;

    return (x_axes_match && y_axes_match)
               ? (b_presses * BUTTON_B_TOKENS + a_presses * BUTTON_A_TOKENS)
               : ULONG_MAX;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");
    unsigned long minimum_tokens_to_win_all_prizes = 0;

    while (true) {
        const size_t LINE_LEN = 50;
        char* const line = malloc(LINE_LEN * sizeof *line);
        fgets(line, LINE_LEN, input_file);

        if (feof(input_file) != false) {
            free(line);
            break;
        } else if (strlen(line) == 1) {
            free(line);
            continue;
        }

        const struct Coord button_a = coord_extract_from_line(line);

        fgets(line, LINE_LEN, input_file);
        const struct Coord button_b = coord_extract_from_line(line);

        fgets(line, LINE_LEN, input_file);
        const struct Coord prize = coord_extract_from_line(line);

        const unsigned long tokens_to_win_prize =
            min_tokens_to_win_prize(button_a, button_b, prize);

        if (tokens_to_win_prize != ULONG_MAX)
            minimum_tokens_to_win_all_prizes += tokens_to_win_prize;

        free(line);
    }

    printf("The minimum tokens needed to win all prizes is %lu.\n",
           minimum_tokens_to_win_all_prizes);

    fclose(input_file);
    return EXIT_SUCCESS;
}
