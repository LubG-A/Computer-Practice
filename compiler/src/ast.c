#include "ast.h"

ASTNode * root = NULL;
token * curr_token;

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
            case PROGRAM: printf("程序：\n"); break;
            case STATEMENT_LIST: printf("语句串：\n"); break;
            case STATEMENT: printf("语句：\n"); break;
            case ASSIGNMENT: printf("赋值语句：\n"); break;
            case IFELSE: printf("条件语句：\n"); break;
            case LOOP: printf("循环语句：\n"); break;
            case EXPRESSION: printf("表达式：\n"); break;
            case TERM: printf("项：\n"); break;
            case FACTOR: printf("因子：\n"); break;
            case CONDITION: printf("条件：\n"); break;
            case CONDITION_OP: printf("条件运算符：\n"); break;
            default: printf("未知类型：\n"); break;
        }
    }
    else {
        switch(node->type.T) {
            case ID: printf("标识符：%s\n", node->value); break;
            case NUM: printf("数字：%s\n", node->value); break;
            case BEGIN: printf("开始：%s\n", node->value); break;
            case IF: printf("如果：%s\n", node->value); break;
            case THEN: printf("那么：%s\n", node->value); break;
            case WHILE: printf("当：%s\n", node->value); break;
            case DO: printf("做：%s\n", node->value); break;
            case END: printf("结束：%s\n", node->value); break;
            case ASSIGN: printf("赋值：%s\n", node->value); break;
            case ADD: printf("加：%s\n", node->value); break;
            case SUB: printf("减：%s\n", node->value); break;
            case MUL: printf("乘：%s\n", node->value); break;
            case DIV: printf("除：%s\n", node->value); break;
            case COLON: printf("冒号：%s\n", node->value); break;
            case LT: printf("小于：%s\n", node->value); break;
            case NEQU: printf("不等于：%s\n", node->value); break;
            case LE: printf("小于等于：%s\n", node->value); break;
            case GT: printf("大于：%s\n", node->value); break;
            case GE: printf("大于等于：%s\n", node->value); break;
            case EQ: printf("等于：%s\n", node->value); break;
            case SEMI: printf("分号：%s\n", node->value); break;
            case LP: printf("左括号：%s\n", node->value); break;
            case RP: printf("右括号：%s\n", node->value); break;
            default: printf("未知类型：\n"); break;
        }
    }
    for(int i = 0; i < node->childCount; i++)
        printAST(node->children[i], level + 1);
}
#endif