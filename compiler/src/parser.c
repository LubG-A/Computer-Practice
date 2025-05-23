#include "parser.h"
#include "ast.h"

token * curr_token;

/** @brief read terminal token in and generate an ASTNode
 *  @param type the expected token type
 */
ASTNode * readTerminal(LEXICON type) {
    getToken();
    if(curr_token->type != type) {
        printf("Syntax Error: Expected %s, got '%s'\n", enum2str[type], curr_token->value);
        exit(1);
    }
    ASTNode * node = allocASTNode(curr_token->value);
    node->type.T = type;
    return node;
}

/** @brief read non-terminal token in and generate an ASTNode
 *  @param node the AST node
 *  @param syntax the syntax function
 */
ASTNode * readNonTerminal(void (*syntax)(ASTNode *), SYNTAX type) {
    ASTNode * node = allocASTNode(NULL);
    node->type.NT = type;
    syntax(node);
    return node;
}

/** @note SELECT(<program> -> begin <statement_list> end) = {begin}
 *  @note No need to look ahead, this is the entrace point of the program
 */
void program(ASTNode * root) {
    branchAST(root, 2);
    root->children[0] = readTerminal(START);
    root->children[1] = readNonTerminal(statement_block, STATEMENT_BLOCK);
    return ;
}

/** @note SELECT(<statement_block> -> '{' <statement_list> '}') = {ID, IF, WHILE}
 *  @param node the AST node
 */
void statement_block(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != LB) {
        printf("Syntax Error: Expected '{', but got '%s'\n", look->value);
        exit(1);
    }
    branchAST(node, 3);
    node->children[0] = readTerminal(LB);
    node->children[1] = readNonTerminal(statement_list, STATEMENT_LIST);
    node->children[2] = readTerminal(RB);
    return ;
}

/** @note SELECT(<statement_list> -> <statement>;{<statement>;}) = {ID, IF, WHILE} 
 *  @param node the AST node
 */
void statement_list(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != ID && look->type != IF && look->type != WHILE) {
        printf("Syntax Error: Expected 'ID', 'IF' or 'WHILE', but got '%s'\n", look->value);
        exit(1);
    }
    
    ASTNode * state = NULL;
    ASTNodeList * head = NULL, * tail = NULL;
    state = readNonTerminal(statement, STATEMENT);
    head = tail = buildASTNodeList(state);

    int count;
    for(count = 1;;count+=1) {
        token * look = lookAheadOne();
        if(look->type != ID && look->type != IF && look->type != WHILE) {
            break;
        }
        state = readNonTerminal(statement, STATEMENT);
        tail = appendASTNodeList(state, tail);
    }

    branchAST(node, count);
    for(int i = 0; i < count; i++) {
        node->children[i] = head->node;
        head = head->next;
    }

    return ;
}

/** @note SELECT(<statement> -> <assignment>) = {ID}
 *  @note SELECT(<statement> -> <ifelse>) = {IF}
 *  @note SELECT(<statement> -> <loop>) = {WHILE}
 *  @param node the AST node
 */
void statement(ASTNode * node) {
    branchAST(node, 1);
    token * look = lookAheadOne();
    if(look->type == ID) {
        node->children[0] = readNonTerminal(assignment, ASSIGNMENT);
    }
    else if(look->type == IF) {
        node->children[0] = readNonTerminal(ifelse, IFELSE);
    }
    else if(look->type == WHILE) {
        node->children[0] = readNonTerminal(loop, LOOP);
    }
    else {
        printf("Syntax Error: Expected 'ID', 'IF' or 'WHILE', but got '%s'\n", look->value);
        exit(1);
    }
    return ;
}

/** @note SELECT(<assignment> -> ID := <expression>;) = {ID}
 *  @param node the AST node
 */
void assignment(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != ID) {
        printf("Syntax Error: Expected 'ID', but got '%s'\n", look->value);
        exit(1);
    }
    branchAST(node, 4);
    node->children[0] = readTerminal(ID);
    node->children[1] = readTerminal(ASSIGN);
    node->children[2] = readNonTerminal(expression, EXPRESSION);
    node->children[3] = readTerminal(SEMI);
    return ;
}

/** @note SELECT(ifelse> -> if <condition> then <statement_list> end) = {if}
 *  @param node the AST node
 */
void ifelse(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != IF) {
        printf("Syntax Error: Expected 'IF', but got '%s'\n", look->value);
        exit(1);
    }
    branchAST(node, 3);
    node->children[0] = readTerminal(IF);
    node->children[1] = readNonTerminal(condition, CONDITION);
    node->children[2] = readNonTerminal(statement_block, STATEMENT_BLOCK);
    return ;
}

/** @note SELECT(<loop> -> while <condition> do <statement_list> end) = {while}
 *  @param node the AST node
 */
void loop(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != WHILE) {
        printf("Syntax Error: Expected 'WHILE', but got '%s'\n", look->value);
        exit(1);
    }
    branchAST(node, 3);
    node->children[0] = readTerminal(WHILE);
    node->children[1] = readNonTerminal(condition, CONDITION);
    node->children[2] = readNonTerminal(statement_block, STATEMENT_BLOCK);
    return ;
}

/** @note SELECT(<expression> -> <term>{+<term> | -<term>}) = {ID, NUM, (}
 *  @param node the AST node
 */
void expression(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != ID && look->type != NUM && look->type != LP) {
        printf("Syntax Error: Expected 'ID', 'NUM' or '(', but got '%s'\n", look->value);
        exit(1);
    }

    ASTNode * trm, * op;
    ASTNodeList * head = NULL, * tail = NULL;
    trm = readNonTerminal(term, TERM);
    head = tail = buildASTNodeList(trm);

    int count;
    for(count = 1;;count+=2) {
        token * look = lookAheadOne();
        if(look->type != ADD && look->type != SUB) {
            break;
        }
        op = readTerminal(look->type);
        tail = appendASTNodeList(op, tail);
        trm = readNonTerminal(term, TERM);
        tail = appendASTNodeList(trm, tail);
    }
    
    branchAST(node, count);
    for(int i = 0; i < count; i++) {
        node->children[i] = head->node;
        head = head->next;
    }
    return ;
}

/** @note SELECT(<term> -> <factor>{*<factor> | /<factor>}) = {ID, NUM, (}
 *  @param node the AST node
 */
void term(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != ID && look->type != NUM && look->type != LP) {
        printf("Syntax Error: Expected 'ID', 'NUM' or '(', but got '%s'\n", look->value);
        exit(1);
    }

    ASTNode * fctr, * op;
    ASTNodeList * head = NULL, * tail = NULL;
    fctr = readNonTerminal(factor, FACTOR);
    head = tail = buildASTNodeList(fctr);

    int count;
    for(count = 1;;count+=2) {
        token * look = lookAheadOne();
        if(look->type != MUL && look->type != DIV) {
            break;
        }
        op = readTerminal(look->type);
        tail = appendASTNodeList(op, tail);
        fctr = readNonTerminal(factor, FACTOR);
        tail = appendASTNodeList(fctr, tail);
    }

    branchAST(node, count);
    for(int i = 0; i < count; i++) {
        node->children[i] = head->node;
        head = head->next;
    }
    return ;
}

/** @note SELECT(<factor> -> ID | NUM | ( <expression> )) = {ID, NUM, (}
 *  @param node the AST node
 */
void factor(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != ID && look->type != NUM && look->type != LP) {
        printf("Syntax Error: Expected 'ID', 'NUM' or '(', but got '%s'\n", look->value);
        exit(1);
    }
    
    if(look->type == ID || look->type == NUM) {
        branchAST(node, 1);
        node->children[0] = readTerminal(look->type);
    }
    else if(look->type == LP) {
        branchAST(node, 3);
        node->children[0] = readTerminal(LP);
        node->children[1] = readNonTerminal(expression, EXPRESSION);
        node->children[2] = readTerminal(RP);
    }
    return ;
}

/** @note SELECT(<condition> -> <expression>{<condition_op> <expression>}) = {ID, NUM, (}
 *  @param node the AST node
 */
void condition(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != ID && look->type != NUM && look->type != LP) {
        printf("Syntax Error: Expected 'ID', 'NUM' or '(', but got '%s'\n", look->value);
        exit(1);
    }

    ASTNode * exp, * op;
    ASTNodeList * head = NULL, * tail = NULL;
    exp = readNonTerminal(expression, EXPRESSION);
    head = tail = buildASTNodeList(exp);

    int count;
    for(count = 1;;count+=2) {
        token * look = lookAheadOne();
        if(look->type != LT && look->type != GT && look->type != EQ && look->type != LE && look->type != GE && look->type != NEQU) {
            break;
        }
        // A blooddy bug here, according to the grammar, you should use readNonTerminal
        // to read the condition_op, but I used readTerminal instead.
        //op = readTerminal(look->type);
        op = readNonTerminal(condition_op, CONDITION_OP);
        tail = appendASTNodeList(op, tail);
        exp = readNonTerminal(expression, EXPRESSION);
        tail = appendASTNodeList(exp, tail);
    }

    branchAST(node, count);
    for(int i = 0; i < count; i++) {
        node->children[i] = head->node;
        head = head->next;
    }
    return ;
}

/** @note SELECT(<condition_op> -> < | > | = | <= | >= | <> ) = {<, >, =, <=, >=, <>}
 *  @param node the AST node
 */
void condition_op(ASTNode * node) {
    token * look = lookAheadOne();
    if(look->type != LT && look->type != GT && look->type != EQ && look->type != LE && look->type != GE && look->type != NEQU) {
        printf("Syntax Error: Expected '<', '>', '=', '<=', '>=', '<>', but got '%s'\n", look->value);
        exit(1);
    }
    branchAST(node, 1);
    node->children[0] = readTerminal(look->type);
    return ;
}