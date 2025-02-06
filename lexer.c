#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 100

// Define token types
typedef enum {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_DIV, TOKEN_GREATER, TOKEN_IF,
    TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_SEMICOLON, TOKEN_UNKNOWN, TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char text[MAX_TOKEN_LEN];
} Token;

// Function to get the next token
void getNextToken(FILE *file, Token *token) {
    int c;
    
    // Skip whitespace
    while ((c = fgetc(file)) != EOF && isspace(c));

    // Check for EOF
    if (c == EOF) {
        token->type = TOKEN_EOF;
        strcpy(token->text, "EOF");
        return;
    }

    // Identify keywords and identifiers
    if (isalpha(c)) {
        int len = 0;
        token->text[len++] = c;
        while (isalnum(c = fgetc(file))) {
            if (len < MAX_TOKEN_LEN - 1) token->text[len++] = c;
        }
        ungetc(c, file);
        token->text[len] = '\0';

        // Check for keywords
        if (strcmp(token->text, "int") == 0) token->type = TOKEN_INT;
        else if (strcmp(token->text, "if") == 0) token->type = TOKEN_IF;
        else token->type = TOKEN_IDENTIFIER;

        return;
    }

    // Identify numbers
    if (isdigit(c)) {
        int len = 0;
        token->text[len++] = c;
        while (isdigit(c = fgetc(file))) {
            if (len < MAX_TOKEN_LEN - 1) token->text[len++] = c;
        }
        ungetc(c, file);
        token->text[len] = '\0';
        token->type = TOKEN_NUMBER;
        return;
    }

    // Identify single-character tokens
    switch (c) {
        case '=': token->type = TOKEN_ASSIGN; strcpy(token->text, "="); break;
        case '+': token->type = TOKEN_PLUS; strcpy(token->text, "+"); break;
        case '-': token->type = TOKEN_MINUS; strcpy(token->text, "-"); break;
        case '*': token->type = TOKEN_MULT; strcpy(token->text, "*"); break;
        case '/': token->type = TOKEN_DIV; strcpy(token->text, "/"); break;
        case '>': token->type = TOKEN_GREATER; strcpy(token->text, ">"); break;
        case '{': token->type = TOKEN_LBRACE; strcpy(token->text, "{"); break;
        case '}': token->type = TOKEN_RBRACE; strcpy(token->text, "}"); break;
        case ';': token->type = TOKEN_SEMICOLON; strcpy(token->text, ";"); break;
        default: token->type = TOKEN_UNKNOWN; token->text[0] = c; token->text[1] = '\0';
    }
}

// Main function to test the lexer
int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    Token token;
    do {
        getNextToken(file, &token);
        printf("Token: %-15d Text: %s\n", token.type, token.text);
    } while (token.type != TOKEN_EOF);

    fclose(file);
    return 0;
}
