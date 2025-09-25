#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

int num_compare(const void* a, const void* b);

int num_compare(const void* a, const void* b) {
    int arg1 = *(const int*) a, arg2 = *(const int*) b;

    if (arg1 < arg2) return -1;
    else if (arg1 > arg2) return 1;
    else return 0;
}

int main(void) {
    int lines_read = 0, list_size = 1000;
    long first_list[list_size], second_list[list_size];
    long distance_total = 0;
    FILE *input_file = fopen("./input/day01.txt", "r");

    for (; true; lines_read++) {
        int line_char_num = 15;
        char line[line_char_num];
        
        if (fgets(line, line_char_num, input_file) == NULL) break;
      
        int args_assigned = sscanf(line, "%ld %ld", &first_list[lines_read], &second_list[lines_read]);
        if (args_assigned != 2) {
            fprintf(stderr, "%s was not able to be read properly.\n", line);
            fclose(input_file);
            return EXIT_FAILURE;
        }
    }

    qsort(first_list, list_size, sizeof(long), num_compare);
    qsort(second_list, list_size, sizeof(long), num_compare);

    for (int count=0; count<list_size; count++) {
        distance_total += labs(first_list[count] - second_list[count]);
    }

    printf("%d lines read!\n", lines_read);
    printf("%ld\n", distance_total);

    fclose(input_file);
    return EXIT_SUCCESS;
}
