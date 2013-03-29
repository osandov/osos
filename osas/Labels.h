#ifndef LABELS_H
#define LABELS_H

/**
 * @file Labels.h
 * The data structure used to store labels defined in an input file
 */

#include "osos/defs.h"

struct LabelTable;

struct Label {
    char *name;
    uword address;
};

/**
 * Create a new table of labels
 */
struct LabelTable *create_label_table();

/**
 * Free a table of labels and any resources it may be using
 */
void free_label_table(struct LabelTable *table);

/**
 * Add a label at the given address to the table
 * @return Zero on success, non-zero on error
 */
int add_label(struct LabelTable *table, char *label, uword address);

/**
 * Look up a label in the tabel
 * @return If the table contains the label, a read-only structure containing the
 * address of the label, otherwise, NULL
 */
const struct Label *get_label(struct LabelTable *table, char *label);

#endif /* LABELS_H */
