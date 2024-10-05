#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100
#define BUFFER_SIZE 1000

typedef enum
{
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN,
    TOKEN_END
} TokenType;

typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "extern", "float", "for", "goto",
    "if", "int", "long", "register", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "include", "define"};

int is_keyword(const char *str)
{
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(str, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int is_operator_char(char c)
{
    return strchr("+-*/=<>!&|%^", c) != NULL;
}

Token get_next_token(const char **input)
{
    Token token;
    token.type = TOKEN_UNKNOWN;
    token.value[0] = '\0';

    while (isspace(**input))
    {
        (*input)++;
    }

    if (**input == '\0')
    {
        token.type = TOKEN_END;
        return token;
    }

    if (**input == '"')
    {
        int length = 0;
        (*input)++;
        while (**input != '"' && **input != '\0' && length < MAX_TOKEN_LENGTH - 1)
        {
            token.value[length++] = **input;
            (*input)++;
        }
        if (**input == '"')
        {
            (*input)++;
        }
        token.value[length] = '\0';
        token.type = TOKEN_STRING_LITERAL;
        return token;
    }

    if (**input == '\'')
    {
        int length = 0;
        (*input)++;
        while (**input != '\'' && **input != '\0' && length < MAX_TOKEN_LENGTH - 1)
        {
            token.value[length++] = **input;
            (*input)++;
        }
        if (**input == '\'')
        {
            (*input)++;
        }
        token.value[length] = '\0';
        token.type = TOKEN_CHAR_LITERAL;
        return token;
    }

    if (isalpha(**input) || **input == '_')
    {
        int length = 0;
        while ((isalnum(**input) || **input == '_') && length < MAX_TOKEN_LENGTH - 1)
        {
            token.value[length++] = **input;
            (*input)++;
        }
        token.value[length] = '\0';
        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        return token;
    }

    if (isdigit(**input))
    {
        int length = 0;
        while (isdigit(**input) && length < MAX_TOKEN_LENGTH - 1)
        {
            token.value[length++] = **input;
            (*input)++;
        }
        token.value[length] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }

    if (is_operator_char(**input) || ispunct(**input))
    {
        token.value[0] = **input;
        token.value[1] = '\0';
        token.type = is_operator_char(**input) ? TOKEN_OPERATOR : TOKEN_PUNCTUATION;
        (*input)++;
        return token;
    }

    token.value[0] = **input;
    token.value[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    (*input)++;

    return token;
}

void print_token(Token token)
{
    const char *typeStr;
    switch (token.type)
    {
    case TOKEN_KEYWORD:
        typeStr = "Keyword";
        break;
    case TOKEN_IDENTIFIER:
        typeStr = "Identifier";
        break;
    case TOKEN_NUMBER:
        typeStr = "Number";
        break;
    case TOKEN_STRING_LITERAL:
        typeStr = "String Literal";
        break;
    case TOKEN_CHAR_LITERAL:
        typeStr = "Char Literal";
        break;
    case TOKEN_OPERATOR:
        typeStr = "Operator";
        break;
    case TOKEN_PUNCTUATION:
        typeStr = "Punctuation";
        break;
    case TOKEN_UNKNOWN:
        typeStr = "Unknown";
        break;
    case TOKEN_END:
        typeStr = "End";
        break;
    default:
        typeStr = "Invalid";
        break;
    }
    printf("Token: %-15s Value: %s\n", typeStr, token.value);
}

int main()
{
    FILE *file = fopen("input.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[bytes_read] = '\0';

    fclose(file);

    const char *current_position = buffer;
    Token token;

    printf("Input:\n%s\n", buffer);
    printf("Tokens:\n");

    while ((token = get_next_token(&current_position)).type != TOKEN_END)
    {
        print_token(token);
    }

    return 0;
}
