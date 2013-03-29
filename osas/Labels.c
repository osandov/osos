#include "Labels.h"
#include <stdlib.h>
#include <string.h>

struct Label *add_label(struct Label *root, struct Label *label)
{
    if (root) {
        int cmp = strcmp(label->name, root->name);
        if (cmp < 0)
            root->left = add_label(root->left, label);
        else if (cmp > 0)
            root->right = add_label(root->right, label);
        else
            label->address = 1;
        return root;
    } else
        return label;
}

struct Label *get_label(struct Label *root, char *label)
{
    if (root) {
        int cmp = strcmp(label, root->name);
        if (cmp < 0)
            return get_label(root->left, label);
        else if (cmp > 0)
            return get_label(root->right, label);
        else
            return root;
    }
    return NULL;
}

void free_labels(struct Label *root)
{
    if (root) {
        free_labels(root->left);
        free_labels(root->right);
        free(root->name);
        free(root);
    }
}
