#include "Buffer.h"
#include "Error.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static FILE **verify_files(char **filenames, int filen);
static struct Buffer *create_buffer(FILE **files, int filen);
static void write_buffer(struct Buffer *buffer, FILE **files);
static size_t total_buffer_size(FILE **files, int filen);

static void close_files(FILE **files, int filen);
static int file_is_dir(FILE *file);
static size_t file_size(FILE *file);

struct Buffer *buffer_files(char **filenames, int filen)
{
    FILE **files = verify_files(filenames, filen);
    struct Buffer *buffer;

    if (!files)
        return NULL;

    buffer = create_buffer(files, filen);
    if (!buffer) {
        close_files(files, filen);
        return NULL;
    }

    buffer->filenames = filenames;
    buffer->filen = filen;

    write_buffer(buffer, files);

    close_files(files, filen);
    return buffer;
}

void free_buffer(struct Buffer *buffer)
{
    free(buffer->buffer);
    free(buffer->line_counts);
    free(buffer);
}

static FILE **verify_files(char **filenames, int filen)
{
    FILE **files = malloc(sizeof(FILE*) * filen);
    int i;
    int err = 0;

    if (files) {
        for (i = 0; i < filen; ++i) {
            FILE *f = fopen(filenames[i], "r");
            if (f && file_is_dir(f)) {
                errno = EISDIR;
                fclose(f);
                f = NULL;
            }
            if (!f) { 
                error("%s: %s\n", filenames[i], strerror(errno));
                err = 1;
            }
            files[i] = f;
        }
        if (err) {
            close_files(files, filen);
            files = NULL;
        }
    }

    return files;
}

static struct Buffer *create_buffer(FILE **files, int filen)
{
    struct Buffer *buffer = (struct Buffer*) malloc(sizeof(struct Buffer));

    if (buffer) {
        buffer->line_counts = (int*) malloc(sizeof(int) * filen);
        if (buffer->line_counts) {
            size_t buffer_size = total_buffer_size(files, filen);
            buffer->buffer = (char*) malloc(buffer_size);
            if (!buffer->buffer) {
                free(buffer->line_counts);
                free(buffer);
                buffer = NULL;
                error("%s\n", strerror(errno));
            }
        } else {
            free(buffer);
            buffer = NULL;
            error("%s\n", strerror(errno));
        }
    } else {
        error("%s\n", strerror(errno));
    }

    return buffer;
}

static void write_buffer(struct Buffer *buffer, FILE **files)
{
    char *p = buffer->buffer;
    int filen = buffer->filen, i;

    for (i = 0; i < filen; ++i) {
        FILE *f = files[i];
        int line_count = 0;
        size_t size = file_size(f), read, j;
        int line_end = 1;

        read = fread(p, 1, size, f);
        for (j = 0; j < read; ++j) {
            if ((line_end = (*p == '\n')))
                ++line_count;
            ++p;
        }
        if (!line_end) {
            *p = '\n';
            ++line_count;
            ++p;
        }
        buffer->line_counts[i] = line_count;
    }
    *p = '\0';
}

static size_t total_buffer_size(FILE **files, int filen)
{
    size_t i, buffer_size = 0;

    for (i = 0; i < filen; ++i)
        buffer_size += file_size(files[i]);

    return buffer_size + filen + 1;
}

static void close_files(FILE **files, int filen)
{
    size_t i;
    for (i = 0; i < filen; ++i) {
        if (files[i])
            fclose(files[i]);
    }
    free(files);
}

static int file_is_dir(FILE *file)
{
    /* TODO: Non-POSIX version */
    struct stat stats;
    fstat(fileno(file), &stats);
    return S_ISDIR(stats.st_mode);
}

static size_t file_size(FILE *file)
{
    /* TODO: Non-POSIX version */
    struct stat stats;
    fstat(fileno(file), &stats);
    return stats.st_size;
}
