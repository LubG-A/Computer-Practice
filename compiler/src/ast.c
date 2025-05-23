#include "ast.h"

ASTNode * root = NULL;

/** @brief branch AST tree 
 *  @note  Use with allocASTNode
*/
void branchAST(ASTNode * node, int childCount) {
    node->childCount = childCount;
    node->children = (ASTNode **)malloc(childCount * sizeof(ASTNode *));
}

/** @brief allocate AST node
 */
ASTNode * allocASTNode(char * value) {
    ASTNode * node = (ASTNode *)malloc(sizeof(ASTNode));
    if(node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    node->childCount = 0;
    node->children = NULL;
    
    if(value != NULL) {
        node->value = (char *)malloc(MAX_TOKEN_LENGTH);
        strcpy(node->value, value);
    }
    else
        node->value = NULL;
    return node;
}

/** @brief build an AST node list
 *  @return initial node, aka the head of the list and the tail
 */
ASTNodeList * buildASTNodeList(ASTNode * node) {
    ASTNodeList * head = (ASTNodeList *)malloc(sizeof(ASTNodeList));
    head->node = node;
    head->next = NULL;
    return head;
}

/** @brief append AST node to the list
 *  @return the tail of the list
 */
ASTNodeList * appendASTNodeList(ASTNode * node, ASTNodeList * tail) {
    ASTNodeList * newNode = (ASTNodeList *)malloc(sizeof(ASTNodeList));
    newNode->node = node;
    newNode->next = NULL;
    tail->next = newNode;
    return tail->next;
}

#ifdef DEBUG_PARSER
void printAST(ASTNode * node, int level) {
    for(int i = 0; i < level; i++)
        printf("    ");
    
    if(isNonTerminal(node)) {
        switch(node->type.NT) {
            case PROGRAM: printf("program: \n"); break;
            case STATEMENT_BLOCK: printf("statement_block: \n"); break;
            case STATEMENT_LIST: printf("statement_list: \n"); break;
            case STATEMENT: printf("statement: \n"); break;
            case ASSIGNMENT: printf("assignment: \n"); break;
            case IFELSE: printf("ifelse: \n"); break;
            case LOOP: printf("loop: \n"); break;
            case EXPRESSION: printf("expression: \n"); break;
            case TERM: printf("term: \n"); break;
            case FACTOR: printf("factor: \n"); break;
            case CONDITION: printf("condition: \n"); break;
            case CONDITION_OP: printf("condition_op: \n"); break;
            default: printf("unknown: \n"); break;
        }
    }
    else {
        switch(node->type.T) {
            case ID: printf("identifier: %s\n", node->value); break;
            case NUM: printf("number: %s\n", node->value); break;
            case START: printf("start: %s\n", node->value); break;
            case IF: printf("if: %s\n", node->value); break;
            case WHILE: printf("while: %s\n", node->value); break;
            case ASSIGN: printf("assign: %s\n", node->value); break;
            case ADD: printf("add: %s\n", node->value); break;
            case SUB: printf("sub: %s\n", node->value); break;
            case MUL: printf("mul: %s\n", node->value); break;
            case DIV: printf("div: %s\n", node->value); break;
            case COLON: printf("colon: %s\n", node->value); break;
            case LT: printf("lt: %s\n", node->value); break;
            case NEQU: printf("nequ: %s\n", node->value); break;
            case LE: printf("le: %s\n", node->value); break;
            case GT: printf("gt: %s\n", node->value); break;
            case GE: printf("ge: %s\n", node->value); break;
            case EQ: printf("eq: %s\n", node->value); break;
            case SEMI: printf("semi: %s\n", node->value); break;
            case LP: printf("lp: %s\n", node->value); break;
            case RP: printf("rp: %s\n", node->value); break;
            case LB: printf("lb: %s\n", node->value); break;
            case RB: printf("rb: %s\n", node->value); break;
            default: printf("unknown: \n"); break;
        }
    }
    for(int i = 0; i < node->childCount; i++)
        printAST(node->children[i], level + 1);
}
#endif