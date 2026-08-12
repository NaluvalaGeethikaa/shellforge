#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "token.h"
#include "lexer.h"
#include "history.h"

int main(void) {
    printf("=========================================\n");
    printf("               Shellforge                \n");
    printf("  A Unix Style Shell written in C       \n");
    printf("=========================================\n");

    char *line = NULL;
    token_list_t token_list;

    while (1) {
        line = readline("shellforge$ ");
        
        if (!line) { // EOF or Ctrl+D
            printf("\nExiting...\n");
            break;
        }

        if (strlen(line) == 0) {
            free(line);
            continue;
        }

        history_add(line);
        add_history(line);

        if (strcmp(line, "exit") == 0) {
            printf("Exiting...\n");
            free(line);
            break;
        }

        if (strcmp(line, "history") == 0) {
            history_print();
            free(line);
            continue;
        }

        // Tokenize and print the output table
        if (lexer(line, &token_list) == 0) {
            token_print(&token_list);
        }

        free(line);
    }

    return 0;
}
