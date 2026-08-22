#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64


/*
 * Parse input into argv-style arguments.
 */
int parse_input(char *input, char **args)
{
    int argc = 0;

    char *token = strtok(input, " \t\r\n");

    while (token != NULL && argc < MAX_ARGS - 1) {

        args[argc] = token;
        argc++;

        token = strtok(NULL, " \t\r\n");
    }

    args[argc] = NULL;

    return argc;
}


/*
 * Display tokens in ShellForge format.
 */
void display_tokens(char **args, int argc)
{
    int i;

    printf("\n-------------- TOKENS --------------\n");

    for (i = 0; i < argc; i++) {

        printf("%d : WORD        %s\n", i, args[i]);
    }

    printf("%d : END         END\n", argc);

    printf("------------------------------------\n");
}


/*
 * Display pipeline information.
 */
void display_pipeline(char **args)
{
    printf("\n========== PIPELINE ==========\n\n");

    printf("Command 1\n");
    printf("------------------------------\n");

    printf("Arguments\n");

    for (int i = 0; args[i] != NULL; i++) {

        printf("argv[%d] = %s\n", i, args[i]);
    }

    printf("Input      : None\n");
    printf("Output     : None\n");
    printf("Append     : No\n");
    printf("Background : No\n");

    printf("==============================\n");
}


/*
 * Main ShellForge loop.
 */
int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {

        /*
         * Shell prompt
         */
        printf("shellforge$ ");
        fflush(stdout);


        /*
         * Read input.
         */
        if (fgets(input, sizeof(input), stdin) == NULL) {

            printf("\n");
            break;
        }


        /*
         * Ignore empty input.
         */
        if (input[0] == '\n') {
            continue;
        }


        /*
         * Parse command.
         */
        int argc = parse_input(input, args);

        if (argc == 0) {
            continue;
        }


        /*
         * Display TOKENS.
         */
        display_tokens(args, argc);


        /*
         * Display PIPELINE.
         */
        display_pipeline(args);


        /*
         * Execute builtin.
         */
        if (builtin_execute(args)) {
            continue;
        }


        /*
         * External commands are not implemented
         * in this milestone.
         */
        printf("shellforge: command not found: %s\n", args[0]);
    }

    return 0;
}

