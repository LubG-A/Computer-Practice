#ifndef AST_H
#define AST_H

#include "common.h"

// From lexer
extern int token_index;
extern token* curr_token;
extern token token_list[MAX_TOKEN_NUM];

extern ASTNode* root;

static inline int isTerminal(ASTNode * node) {
    return node->value != NULL;
}
static inline int isNonTerminal(ASTNode * node) {
    return node->value == NULL;
}

ASTNode* readTerminal(LEXICON type);
ASTNode* readNonTerminal(void (*syntax)(ASTNode*), SYNTAX type);

void branchAST(ASTNode* node, int childCount);
ASTNode* allocASTNode(char* value);
ASTNodeList * buildASTNodeList(ASTNode * node);
ASTNodeList * appendASTNodeList(ASTNode * node, ASTNodeList * tail);

#ifdef DEBUG_PARSER
void printAST(ASTNode* node, int level);
#endif

#endif