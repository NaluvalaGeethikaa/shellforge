#ifndef EXPAND_H
#define EXPAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Maximum sizes */
#define MAX_MACRO_NAME 128
#define MAX_MACRO_VALUE 1024
#define MAX_MACROS 100

/* Macro structure */
typedef struct {
    char name[MAX_MACRO_NAME];
    char value[MAX_MACRO_VALUE];
} Macro;

/* Macro table */
typedef struct {
    Macro macros[MAX_MACROS];
    int count;
} MacroTable;

/* Initialize macro table */
void expand_init(MacroTable *table);

/* Add a macro */
int expand_add_macro(MacroTable *table,
                     const char *name,
                     const char *value);

/* Find a macro */
const char *expand_find_macro(MacroTable *table,
                              const char *name);

/* Expand macros in input */
char *expand_macros(MacroTable *table,
                    const char *input);

/* Remove comments */
char *expand_remove_comments(const char *input);

/* Free allocated memory */
void expand_free(char *result);

#endif
