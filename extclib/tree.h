#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    DECIMAL_ELEM,
    STRING_ELEM,
    REAL_ELEM,
} vtype_tree_t;

typedef union {
    int64_t decimal;
    double real;
    uint8_t *string;
} value_tree_t;

typedef struct tree_node {
    struct data {
        value_tree_t key;
        value_tree_t value;
    } data;

    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *parent;

} tree_node;

typedef struct Tree {
    struct
    {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;

    tree_node *node;

} Tree;

extern Tree *new_tree(vtype_tree_t tkey, vtype_tree_t tvalue);
extern void *decimal(int64_t x);
extern void *string(uint8_t *x);
extern void *real(double x);
extern void print_tree_as_list(Tree *tree);
extern void print_tree(Tree *tree);
extern value_tree_t get_node(Tree *tree, void *key);
extern void del_tree(Tree *tree, void *key);
extern void set_tree(Tree *tree, void *key, void *value);
extern void free_tree(Tree *tree);
extern _Bool in_tree(Tree *tree, void *key);