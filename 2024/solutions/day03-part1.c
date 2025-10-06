#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<regex.h>

struct NumberPair;
int number_pairs_parse_from_line(struct NumberPair* pairs, const char* line);

struct NumberPair {
    long first_num;
    long second_num;
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

    const char* search_text = line;
    int pairs_parsed = 0;
    while(regexec(&regexp, search_text, SUBEXP_COUNT, matches, 0) == 0) {
        pairs[pairs_parsed].first_num = strtol(search_text + matches[1].rm_so, NULL, 10);
        pairs[pairs_parsed].second_num = strtol(search_text + matches[2].rm_so, NULL, 10);

        search_text += matches[0].rm_eo;
        pairs_parsed++;
    }

    regfree(&regexp);
    return pairs_parsed;
}

int main(void) {
    FILE* input_file = fopen("./input.txt", "r");
    long total = 0;

    while(true) {
        const int MAX_LINE_LEN = 4000;
        const int MAX_PAIRS_PER_LINE = 500;
        char* line = malloc(sizeof(char)*(MAX_LINE_LEN+1));
        fgets(line, MAX_LINE_LEN, input_file);

        if (feof(input_file) != 0) {
            free(line);
            break;
        }

        struct NumberPair* pairs = malloc(sizeof(struct NumberPair) * MAX_PAIRS_PER_LINE);
        const int parsed_pairs = number_pairs_parse_from_line(pairs, line);

        for (int index=0; index < parsed_pairs; index++) {
            total += pairs[index].first_num * pairs[index].second_num;
        }
       
        free(pairs);
        free(line);
    }

    printf("Product is %ld\n", total);

    fclose(input_file);
    return EXIT_SUCCESS;
}
