#ifndef FUNC_CACHE_H
#define FUNC_CACHE_H

extern const long FUNC_CACHE_NO_RESULT;

struct FuncCacheTreeNode {
    unsigned long cache_key;
    unsigned long result;
    unsigned long hit_count;

    struct FuncCacheTreeNode* left_node;
    struct FuncCacheTreeNode* right_node;
};

struct FuncCache {
    struct FuncCacheTreeNode* tree_root_node;
    unsigned short tree_left_hits;
    unsigned short tree_right_hits;
};

void func_cache_initialise(struct FuncCache** func_cache);

void func_cache_free(struct FuncCache* const func_cache);

long func_cache_cached_result(const struct FuncCache* const func_cache,
                              const long cache_key);

void func_cache_add_result(struct FuncCache* const func_cache,
                           const long cache_key,
                           const long result);

#endif /* FUNC_CACHE_H */
