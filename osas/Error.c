#include "Error.h"
#include <stdarg.h>
#include <stdio.h>

const char *progname = "osas";

void error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s: ", progname);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void compile_error(const char *filename, int lineno, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s:%d: error: ", filename, lineno);
    vfprintf(stderr, fmt, args);
    va_end(args);
}
