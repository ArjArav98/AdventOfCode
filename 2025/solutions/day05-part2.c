/* Day 05 - Part 2
 * Question Link: https://adventofcode.com/2025/day/5
 * Input Link: https://adventofcode.com/2025/day/5/input
 * */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Range;
struct RangeListNode;
struct RangeList;

struct RangeList* range_list_create(void);
bool ranges_overlap(const struct Range r1, const struct Range r2);
void range_list_trigger_next_node_merge_check(struct RangeListNode* node);
void range_list_insert_node(struct RangeList* range_list, struct RangeListNode* node);
void range_list_node_delete(struct RangeListNode* node);
void range_list_delete(struct RangeList* list);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

struct Range {
    unsigned long start;
    unsigned long end;
};

struct RangeListNode {
    struct Range range;
    struct RangeListNode* next;
};

struct RangeList {
    struct RangeListNode* root;
};

struct RangeList* range_list_create(void) {
    struct RangeList* const list = malloc(sizeof(struct RangeList));
    list->root = NULL;
    return list;
}

bool ranges_overlap(const struct Range r1, const struct Range r2) {
    const bool r2_start_overlaps = (r1.start <= r2.start) && (r1.end >= r2.start);
    const bool r2_end_overlaps = (r1.start <= r2.end) && (r1.end >= r2.end);
    const bool r1_encompasses = (r2.start <= r1.start) && (r2.end >= r1.end);

    return r1_encompasses || r2_start_overlaps || r2_end_overlaps;
}

void range_list_trigger_next_node_merge_check(struct RangeListNode* node) {
    if (node == NULL) return;

    if ((node->next != NULL) && ranges_overlap(node->range, node->next->range)) {
        node->range.start = (node->range.start <= node->next->range.start)
                                ? node->range.start
                                : node->next->range.start;
        node->range.end = (node->range.end >= node->next->range.end)
                              ? node->range.end
                              : node->next->range.end;

        struct RangeListNode* delete_node = node->next;
        node->next = node->next->next;
        free(delete_node);

        range_list_trigger_next_node_merge_check(node);
    }

    range_list_trigger_next_node_merge_check(node->next);
}

void range_list_insert_node(struct RangeList* range_list, struct RangeListNode* node) {
    if (range_list->root == NULL) {
        range_list->root = node;
        return;
    }

    for (struct RangeListNode* curr_node = range_list->root; curr_node != NULL;) {
        if (ranges_overlap(curr_node->range, node->range)) {
            node->next = curr_node->next;
            curr_node->next = node;

            range_list_trigger_next_node_merge_check(curr_node);
            break;
        } else {
            const bool next_node_exists = curr_node->next != NULL;
            const bool after_curr_node = node->range.start > curr_node->range.start;

            if (next_node_exists &&
                (node->range.start <= curr_node->next->range.start)) {
                node->next = curr_node->next;
                curr_node->next = node;

                range_list_trigger_next_node_merge_check(curr_node);
                break;
            } else if (after_curr_node && !next_node_exists) {
                curr_node->next = node;
                break;
            } else curr_node = curr_node->next;
        }
    }
}

void range_list_node_delete(struct RangeListNode* node) {
    if (node == NULL) return;
    range_list_node_delete(node->next);
    free(node);
}

void range_list_delete(struct RangeList* list) {
    range_list_node_delete(list->root);
    free(list);
}

int main(void) {
    FILE* const input_file = fopen("./input.txt", "r");

    /* A RangeList is a variant of the Linked List data structure. It is ordered
     * and triggers updates to successive nodes when a node is updated/inserted.
     * We use this mechanism to stitch together successive ranges if they overlap.
     * */
    struct RangeList* range_list = range_list_create();

    while (true) {
        const size_t LINE_LENGTH = 50;
        char* const line = malloc(LINE_LENGTH * sizeof *line);

        fgets(line, LINE_LENGTH, input_file);

        if (strlen(line) == 1) {
            free(line);
            break;
        }

        char* end_of_first_num = NULL;
        struct Range new_range =
            (struct Range){.start = strtoul(line, &end_of_first_num, 10),
                           .end = strtoul(end_of_first_num + 1, NULL, 10)};

        struct RangeListNode* range_node = malloc(sizeof *range_node);
        *range_node = (struct RangeListNode){.range = new_range, .next = NULL};
        range_list_insert_node(range_list, range_node);

        free(line);
    }

    long long ranges_elements_sum = 0;
    for (struct RangeListNode* curr_node = range_list->root; curr_node != NULL;
         curr_node = curr_node->next) {
        ranges_elements_sum += (curr_node->range.end - curr_node->range.start) + 1;
    }

    printf("The sum of elements in all ranges is %lld\n", ranges_elements_sum);

    range_list_delete(range_list);
    fclose(input_file);
    return EXIT_SUCCESS;
}
