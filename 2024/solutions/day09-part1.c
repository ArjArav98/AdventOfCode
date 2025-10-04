#include <limits.h>
#include <math.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const short FREE_BLOCK = -1;

size_t block_count_from_line(const char* const line);

void disk_fill_with_blocks(short* const disk, const char* const line);

void disk_print(const short* const disk, const size_t nblocks);

unsigned long disk_calculate_checksum(const short* const disk, const size_t nblocks);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

size_t block_count_from_line(const char* const line) {
    size_t blocks = 0, line_len = strlen(line);
    for (size_t i = 0; i < line_len; i++) {
        blocks += (line[i] - '0');
    }
    return blocks;
}

void disk_fill_with_blocks(short* const disk, const char* const line) {
    const size_t line_len = strlen(line);

    bool curr_block_memory_block = true;
    size_t disk_i = 0, memory_block_i = 0;

    for (size_t line_i = 0; line_i < line_len; line_i++) {
        const size_t nmemory_blocks = line[line_i] - '0';

        for (size_t j = 0; j < nmemory_blocks; j++) {
            // Even indexes are memory blocks while odd are free blocks
            disk[disk_i] = (curr_block_memory_block) ? memory_block_i : FREE_BLOCK;
            disk_i++;
        }

        if (curr_block_memory_block) memory_block_i++;
        curr_block_memory_block = !curr_block_memory_block;
    }
}

unsigned long disk_calculate_checksum(const short* const disk, const size_t nblocks) {
    unsigned long checksum = 0;
    for (size_t i = 0; i < nblocks; i++) {
        if (disk[i] == FREE_BLOCK) break;
        checksum += (i * (unsigned long)disk[i]);
    }
    return checksum;
}

void disk_print(const short* const disk, const size_t nblocks) {
    for (size_t i = 0; i < nblocks; i++) {
        if (disk[i] == FREE_BLOCK)
            printf(".");
        else
            printf("%hd", disk[i]);
    }
    printf("\n");
}

int main(void) {
    FILE* input_file = fopen("./input/day09.txt", "r");

    const size_t MAX_LINE_LEN = 20001;
    char* const line = malloc(sizeof(char) * MAX_LINE_LEN);
    fgets(line, MAX_LINE_LEN, input_file);
    line[strlen(line) - 1] = '\0';

    const size_t nblocks = block_count_from_line(line);
    short* const disk = calloc(nblocks, sizeof(short));
    disk_fill_with_blocks(disk, line);

    size_t last_memory_block = nblocks - 1;
    while (true) {
        if (disk[last_memory_block] != FREE_BLOCK) break;
        last_memory_block--;
    }

    for (size_t i = 0, end_i = last_memory_block; end_i >= i; i++) {
        if (disk[i] == FREE_BLOCK) {
            disk[i] = disk[end_i];
            disk[end_i] = FREE_BLOCK;

            while (true) {
                if (disk[end_i] != FREE_BLOCK) break;
                end_i--;
            }
        }
    }

    printf("The final checksum of the disk is %ld\n",
           disk_calculate_checksum(disk, nblocks));

    free(disk);
    free(line);
    fclose(input_file);
    return EXIT_SUCCESS;
}
