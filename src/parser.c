#include "parser.h"

/* Initialize the parser */
void parser_init(Parser *parser, const char *input)
{
    if (parser == NULL)
        return;

    parser->input = input;
    parser->position = 0;
    parser->line = 1;
    parser->column = 1;

    parser->current_token.type = TOKEN_UNKNOWN;
    parser->current_token.value[0] = '\0';
    parser->current_token.line = 1;
    parser->current_token.column = 1;
}

/* Skip spaces, tabs and newlines */
void parser_skip_whitespace(Parser *parser)
{
    if (parser == NULL || parser->input == NULL)
        return;

    while (parser->input[parser->position] != '\0' &&
           isspace((unsigned char)parser->input[parser->position]))
    {
        if (parser->input[parser->position] == '\n')
        {
            parser->line++;
            parser->column = 1;
        }
        else
        {
            parser->column++;
        }

        parser->position++;
    }
}

/* Check whether a character can start an identifier */
int parser_is_identifier_start(char c)
{
    return isalpha((unsigned char)c) || c == '_';
}

/* Check whether a character can be part of an identifier */
int parser_is_identifier_char(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

/* Check whether a word is a keyword */
int parser_is_keyword(const char *str)
{
    const char *keywords[] = {
        "if",
        "else",
        "while",
        "for",
        "int",
        "char",
        "float",
        "double",
        "void",
        "return",
        "struct",
        "break",
        "continue"
    };

    int count = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }

    return 0;
}

/* Get the next token */
Token parser_next_token(Parser *parser)
{
    Token token;

    token.type = TOKEN_UNKNOWN;
    token.value[0] = '\0';
    token.line = parser->line;
    token.column = parser->column;

    /*
     * IMPORTANT:
     * parser is passed as an argument here.
     */
    parser_skip_whitespace(parser);

    token.line = parser->line;
    token.column = parser->column;

    char c = parser->input[parser->position];

    /* End of input */
    if (c == '\0')
    {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");

        parser->current_token = token;
        return token;
    }

    /* Identifier or keyword */
    if (parser_is_identifier_start(c))
    {
        int i = 0;

        while (parser->input[parser->position] != '\0' &&
               parser_is_identifier_char(
                   parser->input[parser->position]))
        {
            if (i < 255)
            {
                token.value[i++] =
                    parser->input[parser->position];
            }

            parser->position++;
            parser->column++;
        }

        token.value[i] = '\0';

        if (parser_is_keyword(token.value))
            token.type = TOKEN_KEYWORD;
        else
            token.type = TOKEN_IDENTIFIER;

        parser->current_token = token;
        return token;
    }

    /* Number */
    if (isdigit((unsigned char)c))
    {
        int i = 0;

        while (parser->input[parser->position] != '\0' &&
               (isdigit((unsigned char)
                        parser->input[parser->position]) ||
                parser->input[parser->position] == '.'))
        {
            if (i < 255)
            {
                token.value[i++] =
                    parser->input[parser->position];
            }

            parser->position++;
            parser->column++;
        }

        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;

        parser->current_token = token;
        return token;
    }

    /* String */
    if (c == '"')
    {
        int i = 0;

        /* Skip opening quote */
        parser->position++;
        parser->column++;

        while (parser->input[parser->position] != '\0' &&
               parser->input[parser->position] != '"')
        {
            if (i < 255)
            {
                token.value[i++] =
                    parser->input[parser->position];
            }

            if (parser->input[parser->position] == '\n')
            {
                parser->line++;
                parser->column = 1;
            }
            else
            {
                parser->column++;
            }

            parser->position++;
        }

        /* Skip closing quote */
        if (parser->input[parser->position] == '"')
        {
            parser->position++;
            parser->column++;
        }

        token.value[i] = '\0';
        token.type = TOKEN_STRING;

        parser->current_token = token;
        return token;
    }

    /* Character literal */
    if (c == '\'')
    {
        int i = 0;

        /* Skip opening quote */
        parser->position++;
        parser->column++;

        while (parser->input[parser->position] != '\0' &&
               parser->input[parser->position] != '\'')
        {
            if (i < 255)
            {
                token.value[i++] =
                    parser->input[parser->position];
            }

            parser->position++;
            parser->column++;
        }

        /* Skip closing quote */
        if (parser->input[parser->position] == '\'')
        {
            parser->position++;
            parser->column++;
        }

        token.value[i] = '\0';
        token.type = TOKEN_STRING;

        parser->current_token = token;
        return token;
    }

    /* Two-character operators */
    if ((c == '=' && parser->input[parser->position + 1] == '=') ||
        (c == '!' && parser->input[parser->position + 1] == '=') ||
        (c == '<' && parser->input[parser->position + 1] == '=') ||
        (c == '>' && parser->input[parser->position + 1] == '=') ||
        (c == '+' && parser->input[parser->position + 1] == '+') ||
        (c == '-' && parser->input[parser->position + 1] == '-') ||
        (c == '&' && parser->input[parser->position + 1] == '&') ||
        (c == '|' && parser->input[parser->position + 1] == '|'))
    {
        token.value[0] = c;
        token.value[1] =
            parser->input[parser->position + 1];
        token.value[2] = '\0';

        token.type = TOKEN_OPERATOR;

        parser->position += 2;
        parser->column += 2;

        parser->current_token = token;
        return token;
    }

    /* Single-character operators */
    if (strchr("+-*/%=<>!&|", c) != NULL)
    {
        token.value[0] = c;
        token.value[1] = '\0';

        token.type = TOKEN_OPERATOR;

        parser->position++;
        parser->column++;

        parser->current_token = token;
        return token;
    }

    /* Symbols */
    if (strchr("(){}[];,.:?", c) != NULL)
    {
        token.value[0] = c;
        token.value[1] = '\0';

        token.type = TOKEN_SYMBOL;

        parser->position++;
        parser->column++;

        parser->current_token = token;
        return token;
    }

    /* Unknown character */
    token.value[0] = c;
    token.value[1] = '\0';
    token.type = TOKEN_UNKNOWN;

    parser->position++;
    parser->column++;

    parser->current_token = token;

    return token;
}

/* Parse the complete input */
int parser_parse(Parser *parser)
{
    if (parser == NULL)
        return 1;

    Token token;

    do
    {
        token = parser_next_token(parser);

        printf("Line %d, Column %d: ",
               token.line,
               token.column);

        switch (token.type)
        {
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER");
                break;

            case TOKEN_NUMBER:
                printf("NUMBER");
                break;

            case TOKEN_OPERATOR:
                printf("OPERATOR");
                break;

            case TOKEN_KEYWORD:
                printf("KEYWORD");
                break;

            case TOKEN_STRING:
                printf("STRING");
                break;

            case TOKEN_SYMBOL:
                printf("SYMBOL");
                break;

            case TOKEN_EOF:
                printf("EOF");
                break;

            case TOKEN_UNKNOWN:
            default:
                printf("UNKNOWN");
                break;
        }

        printf(" -> %s\n", token.value);

    } while (token.type != TOKEN_EOF);

    return 0;
}

/* Display parser error */
void parser_error(Parser *parser, const char *message)
{
    if (parser == NULL)
        return;

    fprintf(stderr,
            "Parser error at line %d, column %d: %s\n",
            parser->line,
            parser->column,
            message);
}
