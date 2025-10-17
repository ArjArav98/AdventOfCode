#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include<stdbool.h>

struct BinaryTreeNode {
    short value;

    struct BinaryTreeNode* left;
    struct BinaryTreeNode* right;
};
void binary_tree_init(struct BinaryTreeNode* root);
void binary_tree_add(struct BinaryTreeNode* const root, const short value);
bool binary_tree_value_exists(const struct BinaryTreeNode* const root, const short value);
void binary_tree_free(const struct BinaryTreeNode* root);

#endif /* BINARY_TREE_H */
