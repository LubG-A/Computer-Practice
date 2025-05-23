#ifndef AST_H
#define AST_H

#include "common.h"

extern ASTNode* root;

static inline int isTerminal(ASTNode * node) {
    return node->value != NULL;
}
static inline int isNonTerminal(ASTNode * node) {
    return node->value == NULL;
}

void branchAST(ASTNode* node, int childCount);
ASTNode* allocASTNode(char* value);
ASTNodeList * buildASTNodeList(ASTNode * node);
ASTNodeList * appendASTNodeList(ASTNode * node, ASTNodeList * tail);

#ifdef DEBUG_PARSER
void printAST(ASTNode* node, int level);
#endif

#endif