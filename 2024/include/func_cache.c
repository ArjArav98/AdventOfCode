#include <stdlib.h>

#include "func_cache.h"

const long FUNC_CACHE_NO_RESULT = -1;

static void func_cache_object_free(struct FuncCacheTreeNode* const node);

/* -------------- */
/* IMPLEMENTATION */
/* -------------- */

/* --------------- */
/* PRIVATE MEMBERS */
/* --------------- */


static void func_cache_object_free(struct FuncCacheTreeNode* const node) {
    if (node == NULL) return;
    if (node->left_node != NULL) func_cache_object_free(node->left_node);
    if (node->right_node != NULL) func_cache_object_free(node->right_node);
    free(node);
}

/* -------------- */
/* PUBLIC MEMBERS */
/* -------------- */

void func_cache_initialise(struct FuncCache** func_cache) {
    *func_cache = malloc(sizeof(struct FuncCache));
    (*func_cache)->tree_root_node = NULL;
}

void func_cache_free(struct FuncCache* const func_cache) {
    func_cache_object_free(func_cache->tree_root_node);
    free(func_cache);
}

long func_cache_cached_result(const struct FuncCache* const func_cache,
                              const long cache_key) {
    struct FuncCacheTreeNode* curr_node = func_cache->tree_root_node;
    while (curr_node != NULL) {
        if (cache_key > curr_node->cache_key)
            curr_node = curr_node->left_node;
        else if (cache_key < curr_node->cache_key)
            curr_node = curr_node->right_node;
        else
            return curr_node->result;
    }

    return FUNC_CACHE_NO_RESULT;
}

void func_cache_add_result(struct FuncCache* const func_cache,
                           const long cache_key,
                           const long result) {
    if (func_cache->tree_root_node == NULL) {
        func_cache->tree_root_node = malloc(sizeof(struct FuncCacheTreeNode));
        *(func_cache->tree_root_node) =
            (struct FuncCacheTreeNode){.cache_key = cache_key,
                                       .result = result,
                                       .left_node = NULL,
                                       .right_node = NULL};
    }

    struct FuncCacheTreeNode* curr_node = func_cache->tree_root_node;

    while (curr_node != NULL) {
        if (curr_node->cache_key == FUNC_CACHE_NO_RESULT) break;

        const struct FuncCacheTreeNode node =
            (struct FuncCacheTreeNode){.cache_key = cache_key,
                                       .result = result,
                                       .left_node = NULL,
                                       .right_node = NULL};

        if (cache_key > curr_node->cache_key) {
            if (curr_node->left_node == NULL) {
                curr_node->left_node = malloc(sizeof(struct FuncCacheTreeNode));
                *(curr_node->left_node) = node;
                break;
            } else
                curr_node = curr_node->left_node;
        } else if (cache_key < curr_node->cache_key) {
            if (curr_node->right_node == NULL) {
                curr_node->right_node = malloc(sizeof(struct FuncCacheTreeNode));
                *(curr_node->right_node) = node;
                break;
            } else
                curr_node = curr_node->right_node;
        } else
            break;
    }
}
