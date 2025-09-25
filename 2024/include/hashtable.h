#ifndef HASHTABLE_H
#define HASHTABLE_H

struct hash_table;

struct hash_table* create_hash_table(void);
void free_hash_table(struct hash_table* table);
void assign_key_value_to_table(struct hash_table* table, int key, int value);
int* key_value_from_table(struct hash_table* table, int key);
int hash_table_nodes_allocated(struct hash_table* table);

#endif /* HASHTABLE_H */
