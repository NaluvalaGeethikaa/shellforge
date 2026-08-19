#include "expand.h"

/* Initialize macro table */
void expand_init(MacroTable *table)
{
    if (table == NULL)
        return;

    table->count = 0;

    for (int i = 0; i < MAX_MACROS; i++)
    {
        table->macros[i].name[0] = '\0';
        table->macros[i].value[0] = '\0';
    }
}

/* Add a macro */
int expand_add_macro(MacroTable *table,
                     const char *name,
                     const char *value)
{
    if (table == NULL || name == NULL || value == NULL)
        return 0;

    /* Check if macro already exists */
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->macros[i].name, name) == 0)
        {
            strncpy(table->macros[i].value,
                    value,
                    MAX_MACRO_VALUE - 1);

            table->macros[i].value[MAX_MACRO_VALUE - 1] = '\0';
            return 1;
        }
    }

    /* Check maximum macro limit */
    if (table->count >= MAX_MACROS)
        return 0;

    strncpy(table->macros[table->count].name,
            name,
            MAX_MACRO_NAME - 1);

    table->macros[table->count].name[MAX_MACRO_NAME - 1] = '\0';

    strncpy(table->macros[table->count].value,
            value,
            MAX_MACRO_VALUE - 1);

    table->macros[table->count].value[MAX_MACRO_VALUE - 1] = '\0';

    table->count++;

    return 1;
}

/* Find a macro */
const char *expand_find_macro(MacroTable *table,
                              const char *name)
{
    if (table == NULL || name == NULL)
        return NULL;

    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->macros[i].name, name) == 0)
            return table->macros[i].value;
    }

    return NULL;
}

/* Check whether character can start an identifier */
static int is_identifier_start(char c)
{
    return isalpha((unsigned char)c) || c == '_';
}

/* Check whether character can be inside an identifier */
static int is_identifier_char(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

/* Expand macros in input */
char *expand_macros(MacroTable *table,
                    const char *input)
{
    if (table == NULL || input == NULL)
        return NULL;

    size_t capacity = strlen(input) + 1024;

    if (capacity < 1024)
        capacity = 1024;

    char *result = malloc(capacity);

    if (result == NULL)
        return NULL;

    result[0] = '\0';

    size_t result_len = 0;
    size_t i = 0;

    while (input[i] != '\0')
    {
        /* Identifier */
        if (is_identifier_start(input[i]))
        {
            char identifier[MAX_MACRO_NAME];
            int j = 0;

            while (is_identifier_char(input[i]))
            {
                if (j < MAX_MACRO_NAME - 1)
                    identifier[j++] = input[i];

                i++;
            }

            identifier[j] = '\0';

            const char *value =
                expand_find_macro(table, identifier);

            const char *text;

            if (value != NULL)
                text = value;
            else
                text = identifier;

            size_t text_len = strlen(text);

            /* Increase buffer if necessary */
            if (result_len + text_len + 1 >= capacity)
            {
                while (result_len + text_len + 1 >= capacity)
                    capacity *= 2;

                char *temp = realloc(result, capacity);

                if (temp == NULL)
                {
                    free(result);
                    return NULL;
                }

                result = temp;
            }

            memcpy(result + result_len,
                   text,
                   text_len);

            result_len += text_len;
        }
        else
        {
            /* Normal character */
            if (result_len + 2 >= capacity)
            {
                capacity *= 2;

                char *temp = realloc(result, capacity);

                if (temp == NULL)
                {
                    free(result);
                    return NULL;
                }

                result = temp;
            }

            result[result_len++] = input[i++];
        }
    }

    result[result_len] = '\0';

    return result;
}

/* Remove C-style comments */
char *expand_remove_comments(const char *input)
{
    if (input == NULL)
        return NULL;

    size_t len = strlen(input);

    char *result = malloc(len + 1);

    if (result == NULL)
        return NULL;

    size_t i = 0;
    size_t j = 0;

    while (i < len)
    {
        /* Single-line comment */
        if (input[i] == '/' && input[i + 1] == '/')
        {
            i += 2;

            while (i < len && input[i] != '\n')
                i++;

            if (i < len)
                result[j++] = input[i++];
        }

        /* Multi-line comment */
        else if (input[i] == '/' && input[i + 1] == '*')
        {
            i += 2;

            while (i < len)
            {
                if (input[i] == '*' && input[i + 1] == '/')
                {
                    i += 2;
                    break;
                }

                /* Preserve newlines */
                if (input[i] == '\n')
                    result[j++] = '\n';

                i++;
            }
        }

        /* Normal character */
        else
        {
            result[j++] = input[i++];
        }
    }

    result[j] = '\0';

    return result;
}

/* Free expanded string */
void expand_free(char *result)
{
    free(result);
}
