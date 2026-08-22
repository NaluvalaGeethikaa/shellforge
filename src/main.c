#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

void parse_input(char *input, char **args)
{
    int i = 0;

    char *token = strtok(input, " \t\n");

    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }

    args[i] = NULL;
}

int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {

        printf("shellforge$ ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        if (input[0] == '\n') {
            continue;
        }

        parse_input(input, args);

        /*
         * Execute builtin commands.
         *
         * builtin_execute returns:
         * 1 -> command was builtin
         * 0 -> command was not builtin
         */

        if (builtin_execute(args)) {
            continue;
        }

        /*
         * For now, non-builtin commands are not handled
         * in this milestone.
         */

        printf("shellforge: command not found: %s\n", args[0]);
    }

    return 0;
}

