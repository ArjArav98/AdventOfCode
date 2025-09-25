#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include"hashtable.h"

void increment_counter_for_key(struct hash_table* counter, int key);

void increment_counter_for_key(struct hash_table* counter, int key) {
    int* counter_value_for_key = key_value_from_table(counter, key);
    
    if (counter_value_for_key == NULL) {
        assign_key_value_to_table(counter, key, 1);
    } else (*counter_value_for_key)++;
}

int main(void) {
    int lines_read = 0, list_size = 1000;
    long first_list[list_size], second_list[list_size];
    struct hash_table* counter = create_hash_table();

    long similarity_score = 0;

    FILE *input_file = fopen("./input/day01.txt", "r");

    for (; true; lines_read++) {
        int line_char_num = 15;
        char line[line_char_num];
        
        if (fgets(line, line_char_num, input_file) == NULL) break;
      
        int args_assigned = sscanf(line, "%ld %ld", &first_list[lines_read], &second_list[lines_read]);
        if (args_assigned != 2) {
            fprintf(stderr, "%s was not able to be read properly.\n", line);
            return EXIT_FAILURE;
        }

        increment_counter_for_key(counter, second_list[lines_read]);
    }

    for (int index=0; index<list_size; index++) {
        int* counter_value_for_key = key_value_from_table(counter, first_list[index]);
        long score = (counter_value_for_key == NULL)? 0 : (long)*counter_value_for_key * (long)first_list[index];
        similarity_score += score;
    }

    printf("%d lines read!\n", lines_read);
    printf("%ld\n", similarity_score);
    return EXIT_SUCCESS;
}
