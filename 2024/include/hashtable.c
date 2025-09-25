#include<stdio.h>
#include<stdlib.h>
#include"hashtable.h"

const int HASH_NODE_CHILDREN = 100;

struct hash_node;

struct hash_node {
    int* value;
    struct hash_node** hash_nodes;
};

struct hash_table {
    struct hash_node* root_node;
};

/* ------------------------------------------- */
/* INTERNAL FUNCTIONS USED BY PUBLIC FUNCTIONS */
/* ------------------------------------------- */

static void assign_key_value_to_node(struct hash_node* root_node, int key, int value) {
    int quotient = key/HASH_NODE_CHILDREN;
    int remainder = key%HASH_NODE_CHILDREN;

    if (root_node->hash_nodes == NULL) {
        root_node->hash_nodes = malloc(sizeof(struct hash_node*)*HASH_NODE_CHILDREN);

        for(int index=0; index<HASH_NODE_CHILDREN; index++) {
            root_node->hash_nodes[index] = malloc(sizeof(struct hash_node));
            root_node->hash_nodes[index]->value = NULL;
            root_node->hash_nodes[index]->hash_nodes = NULL;
        }
    }

    if (quotient == 0) {
        if (root_node->hash_nodes[remainder]->value == NULL) {
            root_node->hash_nodes[remainder]->value = malloc(sizeof(int));
        }

        *(root_node->hash_nodes[remainder]->value) = value;
        return;
    }

    assign_key_value_to_node(root_node->hash_nodes[remainder], quotient, value);
}

static int* key_value_from_node(struct hash_node* root_node, int key) {
    int quotient = key/HASH_NODE_CHILDREN;
    int remainder = key%HASH_NODE_CHILDREN;

    if (quotient == 0) {
        if (root_node->hash_nodes == NULL) return NULL;
        else if (root_node->hash_nodes[remainder]->value == NULL) return NULL;
        else return root_node->hash_nodes[remainder]->value;
    }

    if (root_node->hash_nodes == NULL) return NULL;

    return key_value_from_node(root_node->hash_nodes[remainder], quotient);
}

static int hash_nodes_allocated(struct hash_node* node) {
    if (node->hash_nodes == NULL) return 0;

    int total_count=0;
    for (int index=0; index<HASH_NODE_CHILDREN; index++, total_count++) {
        total_count += hash_nodes_allocated(node->hash_nodes[index]);
    }

    return total_count;
}

static void free_hash_node(struct hash_node* node) {
    if (node->hash_nodes != NULL) {
        for (int index=0; index<HASH_NODE_CHILDREN; index++) {
            free_hash_node(node->hash_nodes[index]);
        }
        free(node->hash_nodes);
    }

    if (node->value != NULL) {
        free(node->value);
    }

    free(node);
}

/* ---------------- */
/* PUBLIC FUNCTIONS */
/* ---------------- */

struct hash_table* create_hash_table(void) {
    struct hash_table* table = malloc(sizeof(struct hash_table));
    table->root_node = malloc(sizeof(struct hash_node));

    table->root_node->value = 0;
    table->root_node->hash_nodes = NULL;

    return table;
}

void free_hash_table(struct hash_table* table) {
    free_hash_node(table->root_node);
    free(table);
}

void assign_key_value_to_table(struct hash_table* table, int key, int value) {
    assign_key_value_to_node(table->root_node, key, value);
}

int* key_value_from_table(struct hash_table* table, int key) {
    return key_value_from_node(table->root_node, key);
}

int hash_table_nodes_allocated(struct hash_table* table) {
    return hash_nodes_allocated(table->root_node);
}
