#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 255
#define MAX_TOKEN_NUM 512

void Initialize();

extern const char * keywords[] = {"begin", "if", "then", "while", "do", "end"};

// Lexical attributes
typedef enum {
    SHARP, BEGIN, IF, THEN, WHILE, DO, END, nul7, nul8, nul9, ID, NUM, nul12, 
    ADD, SUB, MUL, DIV, COLON, ASSIGN, nul19, LT, NEQU, LE, GT, GE, EQ, SEMI, LP, RP, nulldef
} LEXICON;
typedef struct token {
    char* value;
    LEXICON type;
} token;
typedef struct token_read {
    char c;
    int offset;
    char* value;
    LEXICON type;
} token_read;

// Syntax attributes
typedef enum {
    PROGRAM, STATEMENT_LIST, STATEMENT, ASSIGNMENT, IFELSE, LOOP,
    EXPRESSION, TERM, FACTOR, CONDITION, CONDITION_OP
} SYNTAX;

// AST attributes
typedef union {
    SYNTAX NT;
    LEXICON T;
} NT_T;
typedef struct ASTNode {
    NT_T type;
    char* value;         // 如果为NULL，则是非终结符
    int childCount;
    struct ASTNode** children;
} ASTNode;
typedef struct ASTNodeList {
    ASTNode* node;
    struct ASTNodeList* next;
} ASTNodeList;

#endif