#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "expand.h"

#define MAX_INPUT 4096

int main(void)
{
    char input[MAX_INPUT];

    printf("====================================\n");
    printf("        Simple C Parser\n");
    printf("====================================\n");

    printf("Enter C code:\n");
    printf("> ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        fprintf(stderr, "Error: Failed to read input.\n");
        return 1;
    }

    /* Initialize macro table */
    MacroTable table;
    expand_init(&table);

    /* Example macros */
    expand_add_macro(&table, "PI", "3.14");
    expand_add_macro(&table, "SIZE", "10");

    /* Remove comments */
    char *without_comments = expand_remove_comments(input);

    if (without_comments == NULL)
    {
        fprintf(stderr, "Error: Could not remove comments.\n");
        return 1;
    }

    /* Expand macros */
    char *expanded = expand_macros(&table, without_comments);

    if (expanded == NULL)
    {
        fprintf(stderr, "Error: Could not expand macros.\n");
        free(without_comments);
        return 1;
    }

    printf("\n------------------------------------\n");
    printf("After preprocessing:\n");
    printf("------------------------------------\n");
    printf("%s\n", expanded);

    /* Initialize parser */
    Parser parser;

    parser_init(&parser, expanded);

    printf("------------------------------------\n");
    printf("Tokens:\n");
    printf("------------------------------------\n");

    /* Parse input */
    parser_parse(&parser);

    /* Free memory */
    expand_free(without_comments);
    expand_free(expanded);

    printf("------------------------------------\n");
    printf("Parsing completed.\n");
    printf("------------------------------------\n");

    return 0;
}
