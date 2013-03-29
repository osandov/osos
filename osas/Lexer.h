#ifndef LEXER_H
#define LEXER_H

/**
 * @file Lexer.h
 * The lexing and parsing process used to form an abstract syntax tree from
 * buffered input
 */

#include "Buffer.h"

/**
 * A singly-linked list of tokens
 */
struct Token {
    /** The token */
    const char *token;

    /** The next token in the list */
    struct Token *next;
};

/**
 * An abstract syntax tree defined as a singly-linked list of lines, each of
 * which stores a list of tokens
 */
struct Line {
    /** The name of the file containing this line */
    char *filename;

    /** The line number of this line in the file containing it */
    int lineno;

    /** The tokens on this line */
    struct Token *tokens;

    /** The next line in the list */
    struct Line *next;
};

/**
 * Lexes the buffer into lines and tokens
 * @param buffer The buffered input
 * @return The AST generated from the buffer. The last node of the returned list
 * does not store any tokens; if the given buffer contains no tokens, this is
 * the only node in the list. Lines containing no tokens are excluded
 * @note All tokens are copied, so the buffer is not modified and may be freed
 * after calling this function
 */
struct Line *lex(struct Buffer *buffer);

/**
 * Frees a list of lines and their respective lists of tokens
 */
void free_lines(struct Line *front);

/**
 * Frees a list of tokens
 * @note free_lines already frees the tokens stored by each line
 */
void free_tokens(struct Token *front);

#endif /* LEXER_H */
