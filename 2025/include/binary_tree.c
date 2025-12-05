#include "binary_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*--------------------*/
/* PRIVATE PROTOTYPES */
/*--------------------*/

struct BinaryTreeNode;

void binary_tree_node_free(struct BinaryTreeNode* const node);
void binary_tree_node_print(const struct BinaryTreeNode* const node);
bool binary_tree_node_is_leaf(const struct BinaryTreeNode* tree_node);
void binary_tree_node_insert(struct BinaryTreeNode* const root_node,
                             struct BinaryTreeNode* const new_node);
long long
binary_tree_node_get_closest_key_value(const struct BinaryTree* const tree,
                                       const struct BinaryTreeNode* const tree_node,
                                       const long long key);
struct BinaryTreeNode* binary_tree_node_delete(struct BinaryTreeNode* tree_node);

/*------------------------*/
/* PRIVATE IMPLEMENTATION */
/*------------------------*/

struct BinaryTreeNode {
    long long key;
    long long value;

    struct BinaryTreeNode* left;
    struct BinaryTreeNode* right;
};

struct BinaryTree {
    struct BinaryTreeNode* root;
    long long no_result_value;
};

void binary_tree_node_insert(struct BinaryTreeNode* root_node,
                             struct BinaryTreeNode* const new_node) {
    if (new_node == NULL) return;

    if (root_node->key > new_node->key) {
        if (root_node->left == NULL) root_node->left = new_node;
        else binary_tree_node_insert(root_node->left, new_node);
    } else if (root_node->key < new_node->key) {
        if (root_node->right == NULL) root_node->right = new_node;
        else binary_tree_node_insert(root_node->right, new_node);
    } else {
        root_node->value = new_node->value;
        free(new_node);
    }
}

bool binary_tree_node_is_leaf(const struct BinaryTreeNode* tree_node) {
    return (tree_node->left == NULL) && (tree_node->right == NULL);
}

struct BinaryTreeNode* binary_tree_node_delete(struct BinaryTreeNode* delete_node) {
    struct BinaryTreeNode* promoted_node = NULL;

    if (delete_node->right != NULL) {
        binary_tree_node_insert(delete_node->right, delete_node->left);
        delete_node->left = NULL;

        promoted_node = delete_node->right;
    } else promoted_node = delete_node->left;

    free(delete_node);
    return promoted_node;
}

long long
binary_tree_node_get_closest_key_value(const struct BinaryTree* const tree,
                                       const struct BinaryTreeNode* const tree_node,
                                       const long long key) {
    if (tree_node == NULL) return tree->no_result_value;

    if (tree_node->key < key) {
        const long long right_result =
            binary_tree_node_get_closest_key_value(tree, tree_node->right, key);
        if ((right_result != tree->no_result_value) && (right_result < key))
            return right_result;

        return tree_node->value;
    } else if (tree_node->key > key) {
        const long long left_result =
            binary_tree_node_get_closest_key_value(tree, tree_node->left, key);
        if ((left_result != tree->no_result_value) && (left_result < key))
            return left_result;

        return tree->no_result_value;
    } else return tree_node->value;
}

void binary_tree_node_free(struct BinaryTreeNode* const node) {
    if (node == NULL) return;

    binary_tree_node_free(node->left);
    binary_tree_node_free(node->right);

    free(node);
}

void binary_tree_node_print(const struct BinaryTreeNode* const node) {
    if (node == NULL) return;

    binary_tree_node_print(node->left);
    printf("[%lld -> %lld] ", node->key, node->value);
    binary_tree_node_print(node->right);
}

/*-----------------------*/
/* PUBLIC IMPLEMENTATION */
/*-----------------------*/

struct BinaryTree* binary_tree_create(const long long no_result_value) {
    struct BinaryTree* tree = malloc(sizeof(struct BinaryTree));
    tree->root = NULL;
    tree->no_result_value = no_result_value;
    return tree;
}

void binary_tree_insert(struct BinaryTree* tree,
                        const long long key,
                        const long long value) {
    struct BinaryTreeNode* new_node = malloc(sizeof *new_node);
    *new_node = (struct BinaryTreeNode){
        .key = key, .value = value, .left = NULL, .right = NULL};

    if (tree->root == NULL) {
        tree->root = new_node;
        return;
    }

    binary_tree_node_insert(tree->root, new_node);
}

void binary_tree_delete(struct BinaryTree* const tree, const long long key) {
    if (tree->root->key == key) {
        tree->root = binary_tree_node_delete(tree->root);
        return;
    }

    for (struct BinaryTreeNode* curr_node = tree->root; curr_node != NULL;) {
        if (curr_node->key > key) {
            if (curr_node->left != NULL && curr_node->left->key == key)
                curr_node->left = binary_tree_node_delete(curr_node->left);
            else curr_node = curr_node->left;
        } else if (curr_node->key < key) {
            if (curr_node->right != NULL && curr_node->right->key == key)
                curr_node->right = binary_tree_node_delete(curr_node->right);
            else curr_node = curr_node->right;
        }
    }
}

long long binary_tree_get_key_value(const struct BinaryTree* const tree,
                                    const long long key) {
    const struct BinaryTreeNode* curr_node = tree->root;
    while (curr_node != NULL) {
        if (curr_node->key == key) return curr_node->value;
        else if (curr_node->key < key) curr_node = curr_node->right;
        else if (curr_node->key > key) curr_node = curr_node->left;
    }

    return tree->no_result_value;
}

long long binary_tree_get_closest_key_value(const struct BinaryTree* const tree,
                                            const long long key) {
    return binary_tree_node_get_closest_key_value(tree, tree->root, key);
}

void binary_tree_free(struct BinaryTree* tree) {
    binary_tree_node_free(tree->root);
    free(tree);
}

void binary_tree_print(const struct BinaryTree* const tree) {
    binary_tree_node_print(tree->root);
    printf("\n");
}
