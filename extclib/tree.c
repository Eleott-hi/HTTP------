#include "tree.h"

static tree_node *_new_node(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);
static void _set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);
static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value);
static void _print_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static tree_node *_get_node(tree_node *node, vtype_tree_t tkey, void *key);
static void _del_node_1(Tree *tree, tree_node *node);
static void _del_node_2(Tree *tree, tree_node *node);
static void _del_node_3(Tree *tree, tree_node *node);
static void _free_tree(tree_node *node);

/* ============ */
/* EXTERN BEGIN */
/* ============ */

extern void set_tree(Tree *tree, void *key, void *value) {
    if (tree->node == NULL) {
        tree->node = _new_node(tree->type.key, tree->type.value, key, value);
        return;
    }
    _set_tree(tree->node, tree->type.key, tree->type.value, key, value);
}

extern void *decimal(int64_t x) {
    return (void *)x;
}

extern void *string(uint8_t *x) {
    return (void *)x;
}

extern void *real(double x) {
    double *f = malloc(sizeof(double));
    *f = x;
    return (void *)f;
}

extern _Bool in_tree(Tree *tree, void *key) {
    return _get_node(tree->node, tree->type.key, key) != NULL;
}

extern Tree *new_tree(vtype_tree_t tkey, vtype_tree_t tvalue) {
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
    Tree *tree = (Tree *)malloc(sizeof(Tree));
    tree->type.key = tkey;
    tree->type.value = tvalue;
    tree->node = NULL;

    return tree;
};

extern void print_tree_as_list(Tree *tree) {
    putchar('[');
    _print_tree_as_list(tree->node, tree->type.key, tree->type.value);
    putchar(']');
}

extern void print_tree(Tree *tree) {
    _print_tree(tree->node, tree->type.key, tree->type.value);
}

extern value_tree_t get_node(Tree *tree, void *key) {
    tree_node *node = _get_node(tree->node, tree->type.key, key);
    if (node == NULL) {
        fprintf(stderr, "\n\n%s\n\n", "No such key in the binary tree");
        value_tree_t none = {NULL};
        return none;
    }
    return node->data.value;
}

static tree_node *_get_node(tree_node *node, vtype_tree_t tkey, void *key) {
    if (node == NULL) {
        return NULL;
    }

    switch (tkey) {
        case DECIMAL_ELEM:
            if ((int64_t)key < node->data.key.decimal) {
                return _get_node(node->left, tkey, key);
            } else if ((int64_t)key > node->data.key.decimal) {
                return _get_node(node->right, tkey, key);
            } else {
                return node;
            }
            break;

        case STRING_ELEM:
            int cond = strcmp((uint8_t *)key, node->data.key.string);
            if (cond > 0) {
                return _get_node(node->left, tkey, key);
            } else if (cond < 0) {
                return _get_node(node->right, tkey, key);
            } else {
                return node;
            }
            break;
    }

    return NULL;
}

extern void del_tree(Tree *tree, void *key) {
    tree_node *node = _get_node(tree->node, tree->type.key, key);
    if (node == NULL) {
        return;
    }
    if (node->left == NULL && node->right == NULL)
        _del_node_1(tree, node);
    else if (node->left != NULL && node->right != NULL) {
        _del_node_3(tree, node);
    } else {
        _del_node_2(tree, node);
    }
    return;
}

extern void free_tree(Tree *tree) {
    _free_tree(tree->node);
    free(tree);
}

/* ============ */
/* STATIC BEGIN */
/* ============ */

static void _del_node_2(Tree *tree, tree_node *node) {
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            if (node->left != NULL) {
                node->left->parent = node->parent;
                node->parent->left = node->left;
            } else {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
        }
        if (node->parent->right == node) {
            if (node->left != NULL) {
                node->left->parent = node->parent;
                node->parent->right = node->left;
            } else {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }
    } else {
        if (node->left != NULL) {
            tree->node = node->left;
            node->left->parent = NULL;
        } else {
            tree->node = node->right;
            node->right->parent = NULL;
        }
    }
    free(node);
}

static void _del_node_3(Tree *tree, tree_node *node) {
    tree_node *ptr = node;
    ptr = ptr->right;
    char count = 0;
    while (ptr->left != NULL) {
        ptr = ptr->left;
        count = 1;
    }
    node->data.key = ptr->data.key;
    node->data.value = ptr->data.value;

    if (ptr->right != NULL && count) {
        ptr->parent->left = ptr->right;
        ptr->right->parent = ptr->parent;
    }

    if (!count) {
        node->right = ptr->right;
        if (ptr->right != NULL)
            ptr->right->parent = node;
    }
    free(ptr);
}

static void _del_node_1(Tree *tree, tree_node *node) {
    if (node->parent != NULL) {
        if (node->parent->left == node)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
    } else
        tree->node = NULL;

    free(node);
}

static void _set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value) {
    switch (tkey) {
        case DECIMAL_ELEM:
            if ((int64_t)key < node->data.key.decimal) {
                if (node->left == NULL) {
                    node->left = _new_node(tkey, tvalue, key, value);
                    node->left->parent = node;
                } else {
                    _set_tree(node->left, tkey, tvalue, key, value);
                }
            } else if ((int64_t)key > node->data.key.decimal) {
                if (node->right == NULL) {
                    node->right = _new_node(tkey, tvalue, key, value);
                    node->right->parent = node;
                } else {
                    _set_tree(node->right, tkey, tvalue, key, value);
                }
            } else {
                _set_value(node, tvalue, value);
            }
            break;

        case STRING_ELEM:
            int cond = strcmp((uint8_t *)key, node->data.key.string);
            if (cond > 0) {
                if (node->left == NULL) {
                    node->left = _new_node(tkey, tvalue, key, value);
                    node->left->parent = node;
                } else {
                    _set_tree(node->left, tkey, tvalue, key, value);
                }
            } else if (cond < 0) {
                if (node->right == NULL) {
                    node->right = _new_node(tkey, tvalue, key, value);
                    node->right->parent = node;
                } else {
                    _set_tree(node->right, tkey, tvalue, key, value);
                }
            } else {
                _set_value(node, tvalue, value);
            }
            break;
        case REAL_ELEM:
            break;
    }
}

static tree_node *_new_node(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value) {
    tree_node *node = (tree_node *)malloc(sizeof(tree_node));
    switch (tkey) {
        case DECIMAL_ELEM:
            node->data.key.decimal = (int64_t)key;
            _set_value(node, tvalue, value);
            break;

        case STRING_ELEM:
            node->data.key.string = (uint8_t *)key;
            _set_value(node, tvalue, value);
            break;
        case REAL_ELEM:
            break;
    }
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value) {
    switch (tvalue) {
        case DECIMAL_ELEM:
            node->data.value.decimal = (int64_t)value;
            break;
        case REAL_ELEM:
            node->data.value.real = *(double *)value;
            break;
        case STRING_ELEM:
            node->data.value.string = (uint8_t *)value;
            break;
    }
}

static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue) {
    if (node == NULL) {
        return;
    }
    _print_tree_as_list(node->left, tkey, tvalue);
    _print_elem(node, tkey, tvalue);
    _print_tree_as_list(node->right, tkey, tvalue);
}

static void _print_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue) {
    switch (tkey) {
        case DECIMAL_ELEM:
            printf(" [%lld => ", node->data.key.decimal);
            switch (tvalue) {
                case DECIMAL_ELEM:
                    printf("%lld] ", node->data.value.decimal);
                    break;
                case REAL_ELEM:
                    printf("%lf] ", node->data.value.real);
                    break;
                case STRING_ELEM:
                    printf("%s] ", node->data.value.string);
                    break;
            }
            break;

        case STRING_ELEM:
            printf(" [%s => ", node->data.key.string);
            switch (tvalue) {
                case DECIMAL_ELEM:
                    printf("%lld] ", node->data.value.decimal);
                    break;
                case REAL_ELEM:
                    printf("%lf] ", node->data.value.real);
                    break;
                case STRING_ELEM:
                    printf("%s] ", node->data.value.string);
                    break;
            }
        case REAL_ELEM:
            break;
    }
}

static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue) {
    if (node == NULL) {
        printf("null");
        return;
    }
    printf("(");
    _print_tree(node->left, tkey, tvalue);
    _print_elem(node, tkey, tvalue);
    _print_tree(node->right, tkey, tvalue);
    printf(")");
}

static void _free_tree(tree_node *node) {
    if (node == NULL) {
        return;
    }
    _free_tree(node->left);
    _free_tree(node->right);
    free(node);
}