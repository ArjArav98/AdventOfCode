#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdlib.h>

struct BinaryTree;

struct BinaryTree* binary_tree_create(const long long no_result_value);
void binary_tree_insert(struct BinaryTree* tree,
                        const long long key,
                        const long long value);
void binary_tree_delete(struct BinaryTree* tree, const long long key);
long long binary_tree_get_key_value(const struct BinaryTree* const tree,
                                    const long long key);
long long binary_tree_get_closest_key_value(const struct BinaryTree* const tree,
                                            const long long key);
void binary_tree_free(struct BinaryTree* tree);
void binary_tree_print(const struct BinaryTree* const tree);

#endif
