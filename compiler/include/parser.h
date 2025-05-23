#ifndef PARSER_H
#define PARSER_H

#include "common.h"

// From lexer
extern int token_index;
extern token token_list[MAX_TOKEN_NUM];
extern token* curr_token;

static inline void getToken() {
    curr_token = &token_list[token_index++];
}
static inline void ungetToken() {
    curr_token = &token_list[--token_index];
}

static inline token * lookAheadOne() {
    return &token_list[token_index];
}

ASTNode* readTerminal(LEXICON type);
ASTNode* readNonTerminal(void (*syntax)(ASTNode*), SYNTAX type);

void program(ASTNode * root);
void statement_block(ASTNode * node);
void statement_list(ASTNode* node);
void statement(ASTNode* node);
void assignment(ASTNode* node);
void ifelse(ASTNode* node);
void loop(ASTNode* node);
void expression(ASTNode* node);
void term(ASTNode* node);
void factor(ASTNode* node);
void condition(ASTNode* node);
void condition_op(ASTNode* node);

#endif