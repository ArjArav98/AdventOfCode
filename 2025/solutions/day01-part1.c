/* Day 01 - Part 1
 * Question Link: https://adventofcode.com/2025/day/1
 * Input Link: https://adventofcode.com/2025/day/1/input
 * */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

short calculate_dial_num(const short current_dial_num,
                         const short nrotations,
                         const short max_rotations,
                         const char direction);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

short calculate_dial_num(const short current_dial_num,
                         const short nrotations,
                         const short max_rotations,
                         const char direction) {
    const short dial_num_after_rotations = (direction == 'R')
                                               ? current_dial_num + nrotations
                                               : current_dial_num - nrotations;
    // We add 100 to make a negative number positive
    return ((dial_num_after_rotations % max_rotations) + max_rotations) % max_rotations;
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    const short MAX_ROTATIONS = 100;
    short current_dial_num = 50; // initial
    unsigned int zero_count = 0;

    while (true) {
        const size_t MAX_LINE_LENGTH = 10;
        char* const line = malloc((MAX_LINE_LENGTH + 1) * sizeof *line);

        fgets(line, MAX_LINE_LENGTH, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        const char direction = line[0];
        const short nrotations = (short)strtol(line + 1, NULL, 10);

        current_dial_num =
            calculate_dial_num(current_dial_num, nrotations, MAX_ROTATIONS, direction);
        if (current_dial_num == 0) zero_count++;

        free(line);
    }

    printf("The number of times 0 is encountered is %u.\n", zero_count);

    fclose(input_file);
    return EXIT_SUCCESS;
}
