#include "lexer.h"

token_read token_reader;
token token_list[MAX_TOKEN_NUM];
int token_index = 0;
const char * keywords[] = {"begin", "if", "then", "while", "do", "end"};

/** @brief Check token length
 *  @details Check if the token length is greater than MAX_TOKEN_LENGTH
 */
void checkTokenLength() {
    if(token_reader.offset > MAX_TOKEN_LENGTH) {
        printf("Lexical Error: Token can't be longer than %d characters\n", MAX_TOKEN_LENGTH);
        exit(1);
    }
}

/** @brief Analyze a token
 *  @return 0 for EOF, 1 otherwise
 */
int readToken() {
    token_reader.offset = 0;
    token_reader.type = nulldef;
    // Skip whitespace characters
    do {
        token_reader.c = getchar();
    } while(isspace(token_reader.c));

    if(token_reader.c == EOF)
        return 0;

    // The first letter
    token_reader.value[token_reader.offset++] = token_reader.c;

    // If it is a number, then the token must be a number
    if(token_reader.c >= '0' && token_reader.c <= '9') {
        while((token_reader.c = getchar()) != EOF) {
            if(token_reader.c >= '0' && token_reader.c <= '9') {
                token_reader.value[token_reader.offset++] = token_reader.c;
                checkTokenLength();
            }
            else {
                ungetc(token_reader.c, stdin);
                token_reader.value[token_reader.offset] = '\0';
                token_reader.type = NUM;
                break;
            }
        }
    }
    
    // Else It could be Keyword or ID
    else if((token_reader.c >= 'a' && token_reader.c <= 'z') || (token_reader.c >= 'A' && token_reader.c <= 'Z')) {
        while((token_reader.c = getchar()) != EOF) {
            if((token_reader.c >= 'a' && token_reader.c <= 'z') || (token_reader.c >= 'A' && token_reader.c <= 'Z') || (token_reader.c >= '0' && token_reader.c <= '9')) {
                token_reader.value[token_reader.offset++] = token_reader.c;
                checkTokenLength();
            }
            else {
                ungetc(token_reader.c, stdin);
                token_reader.value[token_reader.offset] = '\0';
                token_reader.type = ID;
                // Check if it is a keyword, if it is, then replace the type
                for(int i = 0; i < 6; i++) {
                    if(strcmp(token_reader.value, keywords[i]) == 0) {
                        token_reader.type = BEGIN + i;
                        break;
                    }
                }
                break;
            }
        }
    }

    // Now check if it is an operator
    else if (strchr("+-*/<=>;:()", token_reader.c)) {
        token_reader.c = getchar();
        if(token_reader.c == '=' || token_reader.c == '>') {
            token_reader.value[token_reader.offset++] = token_reader.c;
            token_reader.value[token_reader.offset] = '\0';
            switch(token_reader.value[0]) {
                // :=
                case ':':
                    if(token_reader.value[1] == '=') token_reader.type = ASSIGN;
                    else {printf("Lexical Error: Invalid symbol: %s\nDo you mean ':=' ?", token_reader.value);exit(1);}
                    break;
                // <>
                case '<':
                    if(token_reader.value[1] == '>') token_reader.type = NEQU;
                    else if(token_reader.value[1] == '=') token_reader.type = LE;
                    else {printf("Lexical Error: Invalid symbol: %s\nDo you mean '<>' or '<=' ?", token_reader.value);exit(1);}
                    break;
                // >=
                case '>':
                    if(token_reader.value[1] == '=') token_reader.type = GE;
                    else {printf("Lexical Error: Invalid symbol: %s\nDo you mean '>=' ?", token_reader.value);exit(1);}
                    break;
                default:
                    printf("Lexical Error: Invalid symbol: %s\n", token_reader.value);
                    exit(1);
            }
        }
        else {
            ungetc(token_reader.c, stdin);
            token_reader.value[token_reader.offset] = '\0';
            switch(token_reader.value[0]) {
                case '(':token_reader.type = LP;   break;
                case ')':token_reader.type = RP;   break;
                case '<':token_reader.type = LT;   break;
                case '>':token_reader.type = GT;   break;
                case '=':token_reader.type = EQ;   break;
                case '+':token_reader.type = ADD;  break;
                case '-':token_reader.type = SUB;  break;
                case '*':token_reader.type = MUL;  break;
                case '/':token_reader.type = DIV;  break;
                case ';':token_reader.type = SEMI; break;
                case ':':token_reader.type = COLON;break;
                default:
                    printf("Lexical Error: Invalid symbol: %c\n", token_reader.c);
                    exit(1);
            }
        }
    }

    else if(token_reader.c == '#')
        // Skip comments
        while((token_reader.c = getchar()) != '\n');

    else {
        printf("Lexical Error: Invalid symbol: %c\n", token_reader.c);
        exit(1);
    }

    return 1;
}

/** @brief append token to token list
 *  @param value the token value
 *  @param type the token type
 */
void appendTokenList() {
    token_list[token_index].value = (char *)malloc(MAX_TOKEN_LENGTH);
    strcpy(token_list[token_index].value, token_reader.value);
    token_list[token_index].type = token_reader.type;
    token_index++;
    if(token_index >= MAX_TOKEN_NUM) {
        printf("Error: Too many tokens\n");
        exit(1);
    }
}