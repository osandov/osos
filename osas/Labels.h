#ifndef LABELS_H
#define LABELS_H

/**
 * @file Labels.h
 * The data structure used to store labels defined in an input file
 */

#include "osos/defs.h"

/**
 * A binary search tree of labels
 */
struct Label {
    /** The label string */
    char *name;

    /** The address that the label refers to */
    uword address;

    /** The left subtree */
    struct Label *left;

    /** The right subtree */
    struct Label *right;
};

/**
 * Adds a label to the binary search tree of labels. If the label is a
 * duplicate, the address field will be set to one and the tree will be
 * unchanged
 * @param root The root of the binary search tree
 * @param label The label to add. Only the name field is used when inserting,
 * although the address field may be used as described above
 * @return The new root of the tree
 */
struct Label *add_label(struct Label *root, struct Label *label);

/**
 * Finds a label in the binary search tree
 * @param root The root of the binary search tree
 * @param label The label to search for
 * @return If found, a label matching the given string, otherwise, NULL
 */
struct Label *get_label(struct Label *root, char *label);

/**
 * Frees the label tree
 */
void free_labels(struct Label *root);

#endif /* LABELS_H */
