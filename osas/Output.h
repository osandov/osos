#ifndef OUTPUT_H
#define OUTPUT_H

/**
 * @file Output.h
 * The output production process, producing a bytecode file
 */

#include "Passes.h"
#include <stdio.h>

/**
 * Write the list of opcodes to a file
 * @param file The file to write to
 * @param ops The fully processed list of opcodes
 * @return Zero on success, non-zero on failure
 */
int write_output(FILE *file, struct OpList *ops);

#endif /* OUTPUT_H */
