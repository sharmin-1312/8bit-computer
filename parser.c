#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100

// Token types
typedef enum {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_DIV,
    TOKEN_SEMICOLON, TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char text[MAX_TOKEN_LEN];
} Token;

// Lexer function to read the next token
void getNextToken(FILE *file, Token *token) {
    int c;
    while ((c = fgetc(file)) != EOF && isspace(c)); // Skip spaces

    if (c == EOF) {
        token->type = TOKEN_EOF;
        strcpy(token->text, "EOF");
        return;
    }

    if (isalpha(c)) {
        int len = 0;
        token->text[len++] = c;
        while (isalnum(c = fgetc(file))) {
            if (len < MAX_TOKEN_LEN - 1) token->text[len++] = c;
        }
        ungetc(c, file);
        token->text[len] = '\0';

        if (strcmp(token->text, "int") == 0) token->type = TOKEN_INT;
        else token->type = TOKEN_IDENTIFIER;

        return;
    }

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

    switch (c) {
        case '=': token->type = TOKEN_ASSIGN; strcpy(token->text, "="); break;
        case '+': token->type = TOKEN_PLUS; strcpy(token->text, "+"); break;
        case ';': token->type = TOKEN_SEMICOLON; strcpy(token->text, ";"); break;
        default: token->type = TOKEN_EOF; strcpy(token->text, "EOF");
    }
}

// **Abstract Syntax Tree (AST) Nodes**
typedef struct ASTNode {
    TokenType type;
    char value[MAX_TOKEN_LEN];
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

// **Create a new AST Node**
ASTNode *createNode(TokenType type, char *value, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    strcpy(node->value, value);
    node->left = left;
    node->right = right;
    return node;
}

// **Parse an Assignment Expression (e.g., int x = 5 + 3;)**
ASTNode *parseExpression(FILE *file) {
    Token token;
    getNextToken(file, &token);

    if (token.type == TOKEN_INT) {
        Token varToken;
        getNextToken(file, &varToken);

        if (varToken.type == TOKEN_IDENTIFIER) {
            Token assignToken;
            getNextToken(file, &assignToken);

            if (assignToken.type == TOKEN_ASSIGN) {
                Token valueToken;
                getNextToken(file, &valueToken);

                if (valueToken.type == TOKEN_NUMBER) {
                    // Check for arithmetic operations (e.g., x = 5 + 3)
                    Token nextToken;
                    getNextToken(file, &nextToken);

                    if (nextToken.type == TOKEN_PLUS) {
                        Token secondValue;
                        getNextToken(file, &secondValue);

                        if (secondValue.type == TOKEN_NUMBER) {
                            getNextToken(file, &nextToken); // Expect semicolon

                            if (nextToken.type == TOKEN_SEMICOLON) {
                                return createNode(TOKEN_ASSIGN, "=",
                                    createNode(TOKEN_IDENTIFIER, varToken.text, NULL, NULL),
                                    createNode(TOKEN_PLUS, "+",
                                        createNode(TOKEN_NUMBER, valueToken.text, NULL, NULL),
                                        createNode(TOKEN_NUMBER, secondValue.text, NULL, NULL)
                                    )
                                );
                            }
                        }
                    } else if (nextToken.type == TOKEN_SEMICOLON) {
                        return createNode(TOKEN_ASSIGN, "=",
                            createNode(TOKEN_IDENTIFIER, varToken.text, NULL, NULL),
                            createNode(TOKEN_NUMBER, valueToken.text, NULL, NULL)
                        );
                    }
                }
            }
        }
    }
    return NULL;
}

// **Print AST (for debugging)**
void printAST(ASTNode *node, int level) {
    if (node == NULL) return;
    
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s\n", node->value);
    
    printAST(node->left, level + 1);
    printAST(node->right, level + 1);
}

// **Main Function**
int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    ASTNode *ast = parseExpression(file);
    if (ast) {
        printf("Abstract Syntax Tree:\n");
        printAST(ast, 0);
    } else {
        printf("Syntax error!\n");
    }

    fclose(file);
    return 0;
}
