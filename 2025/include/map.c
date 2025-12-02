#include "map.h"
#include <stdbool.h>
#include <stdio.h>

/*--------------------*/
/* PRIVATE PROTOTYPES */
/*--------------------*/

struct MapNode {
    long long key;
    long long value;

    struct MapNode* left;
    struct MapNode* right;
};

void map_node_free(struct MapNode* const node);
void map_node_print(const struct MapNode* const node);

/*----------------*/
/* IMPLEMENTATION */
/*----------------*/

struct Map {
    struct MapNode* root;
    long long no_result_value;
};

struct Map* map_create(const long long no_result_value) {
    struct Map* map = malloc(sizeof(struct Map));
    map->root = NULL;
    map->no_result_value = no_result_value;
    return map;
}

void map_insert(struct Map* map, const long long key, const long long value) {
    if (map->root == NULL) {
        map->root = malloc(sizeof(struct MapNode));
        *map->root =
            (struct MapNode){.key = key, .value = value, .left = NULL, .right = NULL};
        return;
    }

    struct MapNode* curr_node = map->root;
    while (true) {
        if (curr_node->key == key) {
            curr_node->value = value;
            break;
        } else if (curr_node->key < key) {
            if (curr_node->right == NULL) {
                curr_node->right = malloc(sizeof(struct MapNode));
                *curr_node->right = (struct MapNode){
                    .key = key, .value = value, .left = NULL, .right = NULL};
                break;
            } else curr_node = curr_node->right;
        } else if (curr_node->key > key) {
            if (curr_node->left == NULL) {
                curr_node->left = malloc(sizeof(struct MapNode));
                *curr_node->left = (struct MapNode){
                    .key = key, .value = value, .left = NULL, .right = NULL};
                break;
            } else curr_node = curr_node->left;
        }
    }
}

long long map_get(const struct Map* const map, const long long key) {
    const struct MapNode* curr_node = map->root;
    while (curr_node != NULL) {
        if (curr_node->key == key) return curr_node->value;
        else if (curr_node->key < key) curr_node = curr_node->right;
        else if (curr_node->key > key) curr_node = curr_node->left;
    }

    return map->no_result_value;
}

void map_node_free(struct MapNode* const node) {
    if (node == NULL) return;

    map_node_free(node->left);
    map_node_free(node->right);

    free(node);
}

void map_free(struct Map* map) {
    map_node_free(map->root);
    free(map);
}

void map_node_print(const struct MapNode* const node) {
    if (node == NULL) return;

    map_node_print(node->left);
    printf("[%lld] ", node->value);
    map_node_print(node->right);
}

void map_print(const struct Map* const map) {
    map_node_print(map->root);
    printf("\n");
}
