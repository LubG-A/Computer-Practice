#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "semantic.h"

int main() {
    Initialize();

    // Task 1: Lexical Analysis
    while (1) {
        int ret = readToken();
        if (ret == 0)
            break;
        appendTokenList();
    }
#ifdef DEBUG_LEXER
    for (int i = 0; i < token_index; i++)
        printf("(%d, %s)\n", token_list[i].type, token_list[i].value);
#endif

    // Task 2: Syntax Analysis
    token_index = 0;
    program(root);
#ifdef DEBUG_PARSER
    printAST(root, 0);
#endif

    // Task3: Semantic Analysis
    generate_code(root);
#ifdef DEBUG_SEMANTIC
    print_quadruples();
#endif

    return 0;
}

/** @brief Initialize the tokenizer and AST
 */
void Initialize() {
    token_reader.value = (char *)malloc(MAX_TOKEN_LENGTH);
    if(token_reader.value == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    token_reader.offset = 0;
    token_reader.type = nulldef;
    token_index = 0;

    root = (ASTNode *)malloc(sizeof(ASTNode));
    if(root == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    root->type.NT = PROGRAM;
    root->value = NULL;
}