#include "tree.h"

typedef struct HashTab {
    struct
    {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;

    size_t size;
    Tree **table;

} HashTab;

extern void print_hashtab(HashTab *hashtab);
extern void set_hashtab(HashTab *hashtab, void *key, void *value);
extern HashTab *new_hashtab(size_t size, vtype_tree_t tkey, vtype_tree_t tvalue);
extern void free_hashtab(HashTab *hashtab);
extern void del_hashtab(HashTab *hashtab, void *key);
extern value_tree_t get_hashtab(HashTab *hashtab, void *key);
extern _Bool in_hashtab(HashTab *hashtab, void *key);