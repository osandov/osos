#include "Labels.h"
#include <stdlib.h>
#include <string.h>

struct LabelNode {
    char *name;
    uword address;

    struct LabelNode *left, *right;
};

/**
 * The label table is actually implemented as a binary search tree
 */
struct LabelTable {
    struct LabelNode *root;
};

struct LabelNode *tree_insert(struct LabelNode *root, struct LabelNode *node);
struct LabelNode *tree_search(struct LabelNode *root, char *label);
void tree_free(struct LabelNode *root);

struct LabelTable *create_label_table()
{
    struct LabelTable *table =
        (struct LabelTable*) malloc(sizeof(struct LabelTable));
    table->root = NULL;
    return table;
}

void free_label_table(struct LabelTable *table)
{
    tree_free(table->root);
    free(table);
}

int add_label(struct LabelTable *table, char *label, uword address)
{
    struct LabelNode *node =
        (struct LabelNode*) calloc(1, sizeof(struct LabelNode));
    node->name = label;
    table->root = tree_insert(table->root, node);
    if (node->address) {
        free(node);
        return 1;
    } else {
        node->address = address;
        return 0;
    }
}

const struct Label *get_label(struct LabelTable *table, char *label)
{
    struct LabelNode *node = tree_search(table->root, label);
    return (const struct Label*) node;
}

struct LabelNode *tree_insert(struct LabelNode *root, struct LabelNode *node)
{
    if (root) {
        int cmp = strcmp(node->name, root->name);
        if (cmp < 0)
            root->left = tree_insert(root->left, node);
        else if (cmp > 0)
            root->right = tree_insert(root->right, node);
        else
            node->address = 1;
        return root;
    } else
        return node;
}

struct LabelNode *tree_search(struct LabelNode *root, char *label)
{
    if (root) {
        int cmp = strcmp(label, root->name);
        if (cmp < 0)
            return tree_search(root->left, label);
        else if (cmp > 0)
            return tree_search(root->right, label);
        else
            return root;
    }
    return NULL;
}

void tree_free(struct LabelNode *root)
{
    if (root) {
        tree_free(root->left);
        tree_free(root->right);
        free(root->name);
        free(root);
    }
}
