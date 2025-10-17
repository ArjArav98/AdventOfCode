#include<stdlib.h>
#include<stdbool.h>
#include"binary_tree.h"

void binary_tree_init(struct BinaryTreeNode* root) {
    root->value = -1;
    root->left = NULL;
    root->right = NULL;
}

void binary_tree_add(struct BinaryTreeNode* const root, const short value) {
    struct BinaryTreeNode* curr_node = root;

    while (curr_node != NULL) {
        if (curr_node->value < value) {
            if (curr_node->right == NULL) {
                curr_node->right = malloc(sizeof *curr_node->right);
                *(curr_node->right) =
                    (struct BinaryTreeNode){.value = value, .left = NULL, .right = NULL};
            } else curr_node = curr_node->right;
        } else if (curr_node->value > value) {
            if (curr_node->left == NULL) {
                curr_node->left = malloc(sizeof *curr_node->left);
                *(curr_node->left) =
                    (struct BinaryTreeNode){.value = value, .left = NULL, .right = NULL};
            } else curr_node = curr_node->left;
        } else return;
    }
}

bool binary_tree_value_exists(const struct BinaryTreeNode* const root, const short value) {
    const struct BinaryTreeNode* curr_node = root;
    while (curr_node != NULL) {
        if (curr_node->value < value) curr_node = curr_node->right;
        else if (curr_node->value > value) curr_node = curr_node->left;
        else return true;
    }

    return false;
}

void binary_tree_free(const struct BinaryTreeNode* root) {
    if (root == NULL) return;

    binary_tree_free(root->left);
    binary_tree_free(root->right);

    if (root->right != NULL) free(root->right);
    if (root->left != NULL) free(root->left);
}
