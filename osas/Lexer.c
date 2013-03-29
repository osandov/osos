#include "Lexer.h"
#include "Error.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static struct Line *new_line(struct Line *line);
static size_t count_token_chars(const char *str);
static const char *copy_token(const char *buffer, size_t n);

struct Line *lex(struct Buffer *sbuffer)
{
    struct Line *lines = (struct Line*) calloc(1, sizeof(struct Line));
    struct Line *line = lines;
    struct Token *token = NULL;

    const char *buffer = sbuffer->buffer;
    int filei = 0;
    int lineno = 1;

    if (!lines) {
        error("%s\n", strerror(errno));
        return NULL;
    }

    do {
        size_t n;

        if (*buffer == '\n') {
            ++buffer;
            if (line->tokens) {
                line->filename = sbuffer->filenames[filei];
                line->lineno = lineno;
                line = new_line(line);
                if (!line)
                    goto failure;
            }
            if ((++lineno) > sbuffer->line_counts[filei]) {
                lineno = 1;
                ++filei;
            }
        } else {
            while (*buffer == ' ' || *buffer == '\t')
                ++buffer;
        }

        if (!*buffer)
            break;
        else if (*buffer == '\n')
            continue;

        n = count_token_chars(buffer);

        if (line->tokens)
            token = token->next = (struct Token*) malloc(sizeof(struct Token));
        else
            token = line->tokens = (struct Token*) malloc(sizeof(struct Token));

        if (!token)
            goto failure;

        token->next = NULL;
        token->token = copy_token(buffer, n);

        if (!token->token)
            goto failure;

        buffer += n;
    } while (1);
    return lines;

failure:
    error("%s\n", strerror(errno));
    free_lines(lines);
    return NULL;
}

void free_lines(struct Line *front)
{
    if (front) {
        free_lines(front->next);
        free_tokens(front->tokens);
        free(front);
    }
}

void free_tokens(struct Token *front)
{
    if (front) {
        free_tokens(front->next);
        free((void*) front->token);
        free(front);
    }
}

static struct Line *new_line(struct Line *line)
{
    line = line->next = (struct Line*) malloc(sizeof(struct Line));
    if (line) {
        line->tokens = NULL;
        line->next = NULL;
    }
    return line;
}

static size_t count_token_chars(const char *str)
{
    size_t n = 0;
    while (*str != ' ' && *str != '\t' && *str != '\n') {
        ++str;
        ++n;
    }
    return n;
}

static const char *copy_token(const char *buffer, size_t n)
{
    char *token = (char*) malloc(n + 1);
    if (token) {
        memcpy(token, buffer, n);
        token[n] = '\0';
    }
    return token;
}
