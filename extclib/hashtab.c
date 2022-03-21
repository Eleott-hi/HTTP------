#include "hashtab.h"

static size_t _get_hash(HashTab *hashtab, void *key);
static size_t _get_str_hash(uint8_t *str, size_t size);

extern _Bool in_hashtab(HashTab *hashtab, void *key) {
    size_t hash = _get_hash(hashtab, key);
    return in_tree(hashtab->table[hash], key);
}

extern value_tree_t get_hashtab(HashTab *hashtab, void *key) {
    size_t hash = _get_hash(hashtab, key);
    return get_node(hashtab->table[hash], key);
}

extern void del_hashtab(HashTab *hashtab, void *key) {
    size_t hash = _get_hash(hashtab, key);
    del_tree(hashtab->table[hash], key);
}

extern void free_hashtab(HashTab *hashtab) {
    for (size_t i = 0; i < hashtab->size; i++) {
        free_tree(hashtab->table[i]);
    }
    free(hashtab->table);
    free(hashtab);
}

extern void print_hashtab(HashTab *hashtab) {
    printf("{\n");
    for (size_t i = 0; i < hashtab->size; i++) {
        if (hashtab->table[i]->node == NULL) {
            continue;
        }
        printf("\t");
        printf("%lld => ", i);
        print_tree_as_list(hashtab->table[i]);
        printf("\n");
    }
    printf("}\n");
}

extern void set_hashtab(HashTab *hashtab, void *key, void *value) {
    size_t hash = _get_hash(hashtab, key);
    set_tree(hashtab->table[hash], key, value);
}

extern HashTab *new_hashtab(size_t size, vtype_tree_t tkey, vtype_tree_t tvalue) {
    switch (tkey) {
        case DECIMAL_ELEM:
        case STRING_ELEM:
            break;
        default:
            fprintf(stderr, "\n\n%s\n\n", "Unknown key type");
            return NULL;
    }
    switch (tvalue) {
        case DECIMAL_ELEM:
        case REAL_ELEM:
        case STRING_ELEM:
            break;
        default:
            fprintf(stderr, "\n\n%s\n\n", "Unknown value type");
            return NULL;
    }

    HashTab *hashtab = (HashTab *)malloc(sizeof(HashTab));
    hashtab->type.key = tkey;
    hashtab->type.value = tvalue;
    hashtab->size = size;
    hashtab->table = (Tree **)malloc(size * sizeof(Tree *));

    for (size_t i = 0; i < size; i++) {
        hashtab->table[i] = new_tree(tkey, tvalue);
    }
    return hashtab;
}

static size_t _get_hash(HashTab *hashtab, void *key) {
    size_t hash;
    switch (hashtab->type.key) {
        case DECIMAL_ELEM:
            hash = (int64_t)key % hashtab->size;
            break;
        case STRING_ELEM:
            hash = _get_str_hash((uint8_t *)key, hashtab->size);
            break;
    }
    return hash;
}

static size_t _get_str_hash(uint8_t *str, size_t size) {
    size_t hashval;
    for (hashval = 0; *str != '\0'; ++str) {
        hashval = *str + 31 * hashval;
    }
    return hashval % size;
}