#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<regex.h>

struct NumberPair;
int number_pairs_parse_from_line(struct NumberPair* pairs, const char* line);
int string_offsets_in_line(regoff_t* offsets, const char* line, const char* string);

struct NumberPair {
    long first_num;
    long second_num;
    regoff_t input_line_offset;
};

int number_pairs_parse_from_line(struct NumberPair* pairs, const char* line) {
    const int SUBEXP_COUNT = 3;
    regex_t regexp;
    regmatch_t matches[SUBEXP_COUNT];
    const char* pattern = "mul(\\([[:digit:]]\\{1,3\\}\\),\\([[:digit:]]\\{1,3\\}\\))";
    
    if(regcomp(&regexp, pattern, 0) != 0) {
        fprintf(stderr, "Could not compile regular expression");
        regfree(&regexp);
        return 0;
    }

    regoff_t last_offset = 0;
    const char* search_text = line;
    int pairs_parsed = 0;
    while(regexec(&regexp, search_text, SUBEXP_COUNT, matches, 0) == 0) {
        pairs[pairs_parsed].first_num = strtol(search_text + matches[1].rm_so, NULL, 10);
        pairs[pairs_parsed].second_num = strtol(search_text + matches[2].rm_so, NULL, 10);
        pairs[pairs_parsed].input_line_offset = last_offset + matches[0].rm_so;

        search_text += matches[0].rm_eo;
        last_offset += matches[0].rm_eo;
        pairs_parsed++;
    }

    regfree(&regexp);
    return pairs_parsed;
}

int string_offsets_in_line(regoff_t* offsets, const char* line, const char* string) {
    regex_t regexp;
    regmatch_t matches[1];
    
    if(regcomp(&regexp, string, 0) != 0) {
        fprintf(stderr, "Could not compile regular expression");
        regfree(&regexp);
        return 0;
    }

    const char* search_text = line;
    regoff_t last_offset = 0;
    int string_occurrences = 0;
    while(regexec(&regexp, search_text, 1, matches, 0) == 0) {
        offsets[string_occurrences] = last_offset + matches[0].rm_so;

        string_occurrences++;
        search_text += matches[0].rm_eo;
        last_offset += matches[0].rm_eo;
    }

    regfree(&regexp);
    return string_occurrences;
}

int main(void) {
    FILE* input_file = fopen("./input.txt", "r");
    long total = 0;

    bool is_counted = true;
    while(true) {
        const int MAX_LINE_LEN = 4000;
        const int MAX_PAIRS_PER_LINE = 500;
        const int MAX_DO_DONTS_PER_LINE = 600;
        char* line = malloc(sizeof(char)*(MAX_LINE_LEN+1));
        fgets(line, MAX_LINE_LEN, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        regoff_t* do_offsets_in_line = malloc(sizeof(regoff_t) * MAX_DO_DONTS_PER_LINE);
        regoff_t* dont_offsets_in_line = malloc(sizeof(regoff_t) * MAX_DO_DONTS_PER_LINE);
        const int do_count = string_offsets_in_line(do_offsets_in_line, line, "do()");
        const int dont_count = string_offsets_in_line(dont_offsets_in_line, line, "don't()");

        struct NumberPair* pairs = malloc(sizeof(struct NumberPair) * MAX_PAIRS_PER_LINE);
        const int parsed_pairs = number_pairs_parse_from_line(pairs, line);

        for (
            size_t line_index=0, do_index=0, dont_index=0, pair_index=0;
            line_index < strlen(line);
            line_index++
        ) {
            if ((do_index < do_count) && (do_offsets_in_line[do_index] == line_index)) {
                is_counted = true;
                do_index++;
            }
            if ((dont_index < dont_count) && (dont_offsets_in_line[dont_index] == line_index)) {
                is_counted = false;
                dont_index++;
            }
            if ((pair_index < parsed_pairs) && (pairs[pair_index].input_line_offset == line_index)) {
                if (is_counted) {
                    total += pairs[pair_index].first_num * pairs[pair_index].second_num;
                }
                pair_index++;
            }
        }
       
        free(do_offsets_in_line);
        free(dont_offsets_in_line);
        free(pairs);
        free(line);
    }

    printf("Product is %ld\n", total);

    fclose(input_file);
    return EXIT_SUCCESS;
}
