#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

int lexer(const char *input, token_list_t *list) {
    token_list_init(list);
    int i = 0;

    while (input[i] != '\0') {
        // Skip whitespaces
        if (isspace((unsigned char)input[i])) {
            i++;
            continue;
        }

        // Pipe operator '|'
        if (input[i] == '|') {
            token_add(list, TOKEN_PIPE, "|");
            i++;
            continue;
        }

        // Input redirection '<'
        if (input[i] == '<') {
            token_add(list, TOKEN_INPUT, "<");
            i++;
            continue;
        }

        // Output redirection '>' or '>>'
        if (input[i] == '>') {
            if (input[i + 1] == '>') {
                token_add(list, TOKEN_APPEND, ">>");
                i += 2;
            } else {
                token_add(list, TOKEN_OUTPUT, ">");
                i++;
            }
            continue;
        }

        // Background execution '&'
        if (input[i] == '&') {
            token_add(list, TOKEN_BACKGROUND, "&");
            i++;
            continue;
        }

        // WORD Token Parsing
        char word[MAX_TOKEN_LEN];
        int j = 0;

        while (input[i] != '\0' && !isspace((unsigned char)input[i]) &&
               input[i] != '|' && input[i] != '<' && input[i] != '>' && input[i] != '&') {

            // Single Quote
            if (input[i] == '\'') {
                i++; // skip opening single quote
                while (input[i] != '\0' && input[i] != '\'') {
                    if (j < MAX_TOKEN_LEN - 1) {
                        word[j++] = input[i];
                    }
                    i++;
                }
                if (input[i] == '\'') {
                    i++; // skip closing single quote
                } else {
                    printf("Lexer Error : Unterminated single quote\n");
                    return -1;
                }
            }
            // Double Quote
            else if (input[i] == '"') {
                i++; // skip opening double quote
                while (input[i] != '\0' && input[i] != '"') {
                    if (input[i] == '\\' && input[i + 1] != '\0') {
                        i++; // skip escape char '\'
                    }
                    if (j < MAX_TOKEN_LEN - 1) {
                        word[j++] = input[i];
                    }
                    i++;
                }
                if (input[i] == '"') {
                    i++; // skip closing double quote
                } else {
                    printf("Lexer Error : Unterminated double quote\n");
                    return -1;
                }
            }
            // Escape Character
            else if (input[i] == '\\') {
                i++; // skip '\'
                if (input[i] != '\0') {
                    if (j < MAX_TOKEN_LEN - 1) {
                        word[j++] = input[i];
                    }
                    i++;
                }
            }
            // Normal Character
            else {
                if (j < MAX_TOKEN_LEN - 1) {
                    word[j++] = input[i];
                }
                i++;
            }
        }

        word[j] = '\0';
        if (j > 0) {
            token_add(list, TOKEN_WORD, word);
        }
    }

    token_add(list, TOKEN_END, "END");
    return 0;
}
