#ifndef MAP_C
#define MAP_C

#include <stdlib.h>

struct Map;

struct Map* map_create(const long long no_result_value);
void map_insert(struct Map* map, const long long key, const long long value);
long long map_get(const struct Map* const map, const long long key);
void map_free(struct Map* map);
void map_print(const struct Map* const map);

#endif
