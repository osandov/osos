#ifndef BUFFER_H
#define BUFFER_H

/**
 * @file Buffer.h
 * The buffering process used to prepare input for lexing
 */

#include <stddef.h>

/**
 * A structure storing a buffer of the files to process, along with statistics
 * for each file in order to allow for helpful error reporting
 */
struct Buffer {
    /** A null-terminated string storing the contents of each file */
    char *buffer;

    /** An array of the names of the files stored */
    char **filenames;

    /** An array of the number of lines in each file */
    int *line_counts;

    /** The number of files in the buffer */
    int filen;
};

/**
 * Buffers zero or more files into one contiguous string while storing
 * auxiliary data
 * @param filenames An array of the names of the files to buffer
 * @param filen The number of files to buffer
 * @note If any file does not end with a newline, one is appended in the buffer
 */ 
struct Buffer *buffer_files(char **filenames, int filen);

/**
 * Frees a buffer and its underlying resources
 */
void free_buffer(struct Buffer *buffer);

#endif /* BUFFER_H */
