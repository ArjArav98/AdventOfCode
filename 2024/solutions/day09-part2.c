#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const short FREE_BLOCK = -1;

size_t block_count_from_line(const char* const line);

void disk_fill_with_blocks(short* const disk, const char* const line);

void disk_fill_block_with_value(short* const disk, const size_t block_start_i,
                                const size_t block_len, const short value);

unsigned long disk_calculate_checksum(const short* const disk, const size_t nblocks);

size_t disk_calculate_memory_block_start(const short* const disk, const size_t index);

size_t disk_find_free_space_offset(const short* const disk, const size_t nblocks,
                                   const size_t free_space_length);

void disk_print(const short* const disk, const size_t nblocks);

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

void disk_fill_block_with_value(short* const disk, const size_t block_start_i,
                                const size_t block_len, const short value) {
    for (size_t i = 0; i < block_len; i++)
        disk[i + block_start_i] = value;
}

size_t disk_calculate_memory_block_start(const short* const disk, const size_t index) {
    size_t length = 0;
    const short curr_memory_block_id = disk[index];
    for (size_t disk_i = index; disk[disk_i] == curr_memory_block_id;
         disk_i--, length++)
        ;
    return length;
}

size_t disk_find_free_space_offset(const short* const disk, const size_t nblocks,
                                   const size_t free_space_length) {
    for (size_t i = 0, length = 0; i < nblocks; i++) {
        if (disk[i] == FREE_BLOCK) {
            length++;
            if (length == free_space_length) return i - (length - 1);
        } else
            length = 0;
    }
    return nblocks;
}

unsigned long disk_calculate_checksum(const short* const disk, const size_t nblocks) {
    unsigned long checksum = 0;
    for (size_t i = 0; i < nblocks; i++) {
        if (disk[i] == FREE_BLOCK) continue;
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
    FILE* input_file = fopen("./input.txt", "r");

    const size_t MAX_LINE_LEN = 20001;
    char* const line = malloc(sizeof(char) * MAX_LINE_LEN);
    fgets(line, MAX_LINE_LEN, input_file);
    line[strlen(line) - 1] = '\0';

    const size_t nblocks = block_count_from_line(line);
    short* const disk = calloc(nblocks, sizeof(short));
    disk_fill_with_blocks(disk, line);

    for (size_t i = nblocks - 1; i >= 0; i--) {
        if (disk[i] == FREE_BLOCK) continue;

        const size_t curr_file_block_len = disk_calculate_memory_block_start(disk, i);
        if (curr_file_block_len >= i) break;
        const size_t curr_file_block_start = i - (curr_file_block_len - 1);

        const size_t free_space_start =
            disk_find_free_space_offset(disk, nblocks, curr_file_block_len);
        if (free_space_start >= curr_file_block_start) {
            i = curr_file_block_start;
            continue;
        }

        disk_fill_block_with_value(disk, free_space_start, curr_file_block_len,
                                   disk[i]);
        disk_fill_block_with_value(disk, curr_file_block_start, curr_file_block_len,
                                   FREE_BLOCK);

        i = curr_file_block_start;
    }

    printf("The final checksum of the disk is %ld\n",
           disk_calculate_checksum(disk, nblocks));

    free(disk);
    free(line);
    fclose(input_file);
    return EXIT_SUCCESS;
}
