#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 255
#define MAX_TOKEN_NUM 512

#define MAX_QUADRUPLE 255

#define DEBUG_PARSER

void Initialize();

extern const char * keywords[];

// Lexical attributes
typedef enum {
    nulldef, START, IF, WHILE, ID, NUM,
    ADD, SUB, MUL, DIV, LT, GT, LE, GE, EQ, NEQU, ASSIGN,
    SHARP, COLON, SEMI, LP, RP, LB, RB,
    GOTO, IFGOTO, LABEL
} LEXICON;
extern const char * enum2str[];
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
    PROGRAM, STATEMENT_BLOCK, STATEMENT_LIST, STATEMENT, ASSIGNMENT, IFELSE,
    LOOP, EXPRESSION, TERM, FACTOR, CONDITION, CONDITION_OP
} SYNTAX;

// AST attributes
typedef union {
    SYNTAX NT;
    LEXICON T;
} NT_T;
typedef struct ASTNode {
    NT_T type;
    char* value;
    int childCount;
    struct ASTNode** children;
} ASTNode;
typedef struct ASTNodeList {
    ASTNode* node;
    struct ASTNodeList* next;
} ASTNodeList;

// Semtantic attributes
typedef union {
    char* arg;
    int label;
} Result;
typedef struct {
    const char * arg1;
    const char * arg2;
    Result result;
    LEXICON op;
} Quadruple;

#endif