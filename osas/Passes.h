#ifndef PASSES_H
#define PASSES_H

/**
 * @file Passes.h
 * Passes over the abstract syntax tree
 */

#include "osos/OpCodes.h"
#include "Labels.h"
#include "Lexer.h"

/**
 * A list of opcodes, storing auxiliary data for error reporting
 */
struct OpList {
    /** The opcode */
    struct OpCode opcode;

    /** The line from which the opcode was generated */;
    struct Line *line;

    /** The next opcode in the list */
    struct OpList *next;
};

/**
 * The first pass over the AST. Converts the AST to a list of opcodes and a
 * binary search tree of labels
 * @param [in] lines The AST to parse
 * @param [out] ops The generated list of opcodes
 * @param [out] labels The generated binary search tree of labels
 * @return Zero on success, non-zero on error
 * @note lines shouldn't be freed until after the second pass
 */
int first_pass(struct Line *lines, struct OpList **ops, struct Label **labels);

/**
 * The second and final pass over the AST. Resolves labels to constant offsets
 * @param ops The list of opcodes to process
 * @param labels The label tree to use when resolving
 * @return Zero on success, non-zero on error
 */
int second_pass(struct OpList *ops, struct Label *labels);

/**
 * Frees a list of opcodes
 */
void free_oplist(struct OpList *front);

#endif /* PASSES_H */
