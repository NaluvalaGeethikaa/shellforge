#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Token types */
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_SYMBOL,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

/* Token structure */
typedef struct {
    TokenType type;
    char value[256];
    int line;
    int column;
} Token;

/* Parser structure */
typedef struct {
    const char *input;
    int position;
    int line;
    int column;
    Token current_token;
} Parser;

/* Initialize parser */
void parser_init(Parser *parser, const char *input);

/* Get next token */
Token parser_next_token(Parser *parser);

/* Parse input */
int parser_parse(Parser *parser);

/* Utility functions */
void parser_skip_whitespace(Parser *parser);
int parser_is_identifier_start(char c);
int parser_is_identifier_char(char c);
int parser_is_keyword(const char *str);

/* Error handling */
void parser_error(Parser *parser, const char *message);

#endif /* PARSER_H */
