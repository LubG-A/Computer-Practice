#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "common.h"

// From AST
extern ASTNode * root;

extern int quad_count;
extern Quadruple quadruples[];

static inline char* new_temp() {
    static int temp_count = 0;
    char* temp = (char*)malloc(4);
    sprintf(temp, "t%d", ++temp_count);
    return temp;
}
static inline int new_label() {
    static int label_count = 0;;
    return ++label_count;
}

void add_quadruple(LEXICON op, const char* arg1, const char* arg2, Result result);
void generate_code(ASTNode* node);
char* generate_expression(ASTNode* node);
char* generate_term(ASTNode* node);
char* generate_factor(ASTNode* node);
char* generate_condition(ASTNode* node);

#ifdef DEBUG_SEMANTIC
void print_quadruples();
#endif

#endif