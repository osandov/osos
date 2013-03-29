#ifndef ASSEMBLE_H
#define ASSEMBLE_H

/**
 * @file Assemble.h
 * The main assembler definitions
 */

/**
 * Assemble input files into bytecode
 * @param input_files An array of the names of the input files to assemble
 * @param filen The number of input files to assemble
 * @param output_file The file to write the bytecode output to
 */
int assemble(char **input_files, int filen, char *output_file);

#endif /* ASSEMBLE_H */
