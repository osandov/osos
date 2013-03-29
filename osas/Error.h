#ifndef ERROR_H
#define ERROR_H

/**
 * @file Error.h
 * Error reporting mechanisms
 */

/** The name of the program used when printing program errors */
extern const char *progname;

/**
 * Prints a formatted error to stderr prepended with the name of the program
 * @param fmt A printf-style format string
 * @param ... The values for the format specifiers
 */
void error(const char *fmt, ...);

/**
 * Prints a formatted error to stderr prepended with the name of the file and
 * the line where the error occurred
 * @param filename The name of the file containing the error
 * @param lineno The number of the line containing the error
 * @param fmt A printf-style format string
 * @param ... The values for the format specifiers
 */
void compile_error(const char *filename, int lineno, const char *fmt, ...);

#endif /* ERROR_H */
