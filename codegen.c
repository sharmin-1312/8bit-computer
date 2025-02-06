#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define token types
typedef enum {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_DIV,
    TOKEN_SEMICOLON, TOKEN_EOF
} TokenType;

// Define AST Node structure
typedef struct ASTNode {
    TokenType type;
    char value[100];
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

// Function to create an AST Node
ASTNode *createNode(TokenType type, char *value, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    strcpy(node->value, value);
    node->left = left;
    node->right = right;
    return node;
}

// Function to generate assembly code from AST
void generateAssembly(ASTNode *node) {
    if (node == NULL) return;

    // If node is a number, load it into the accumulator
    if (node->type == TOKEN_NUMBER) {
        printf("LOAD %s\n", node->value);
        return;
    }

    // If node is an assignment (x = something)
    if (node->type == TOKEN_ASSIGN) {
        generateAssembly(node->right); // Process the right-hand side first
        printf("STORE %s\n", node->left->value); // Store the result in the variable
        return;
    }

    // If node is an addition (5 + 3)
    if (node->type == TOKEN_PLUS) {
        generateAssembly(node->left);  // Load left operand (5)
        printf("ADD %s\n", node->right->value); // Add right operand (3)
        return;
    }
}

// Function to print the AST (for debugging)
void printAST(ASTNode *node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s\n", node->value);
    printAST(node->left, level + 1);
    printAST(node->right, level + 1);
}

// **Main Function**
int main() {
    // Example AST for: int x = 5 + 3;
    ASTNode *ast = createNode(TOKEN_ASSIGN, "=",
        createNode(TOKEN_IDENTIFIER, "x", NULL, NULL),
        createNode(TOKEN_PLUS, "+",
            createNode(TOKEN_NUMBER, "5", NULL, NULL),
            createNode(TOKEN_NUMBER, "3", NULL, NULL)
        )
    );

    // Print AST structure
    printf("Abstract Syntax Tree:\n");
    printAST(ast, 0);

    // Generate and print assembly code
    printf("\nGenerated Assembly Code:\n");
    generateAssembly(ast);

    return 0;
}
