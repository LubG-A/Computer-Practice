#ifndef LEXER_H
#define LEXER_H

#include "common.h"

extern int token_index;
extern token_read token_reader;
extern token token_list[MAX_TOKEN_NUM];

// 函数声明
int readToken();
void appendTokenList();
void checkTokenLength();

#endif