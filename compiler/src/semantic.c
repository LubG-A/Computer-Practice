#include "semantic.h"
#include "ast.h"

int quad_count = 0;
Quadruple quadruples[MAX_QUADRUPLE];

/** @brief add elements to quadruples
 *  @param result = arg1 op arg2
 */
void add_quadruple(LEXICON op, const char* arg1, const char* arg2, Result result) {
    quadruples[quad_count].op = op;
    quadruples[quad_count].arg1 = arg1;
    quadruples[quad_count].arg2 = arg2;
    quadruples[quad_count].result = result;
    quad_count++;
}

/** @brief 遍历AST生成中间代码 */
void generate_code(ASTNode* node) {
    if (!node) return;
    if (isTerminal(node)) return;

    switch (node->type.NT) {
        case ASSIGNMENT:
            // var := expression
            /*  t_result = expression
                var := t_result
            */
            {
                char* id = node->children[0]->value;
                char* expr_result = generate_expression(node->children[2]);

                add_quadruple(ASSIGN, expr_result, "", (Result){.arg = id});
            }
            break;

        case IFELSE:
            // if condition statement_block
            /*  t_result = condition
                ifFalse t_result goto Label
                statement_block
                Label:
            */
            {
                int label_end = new_label();
                char* condition_result = generate_condition(node->children[1]);

                add_quadruple(IFGOTO, condition_result, "0", (Result){.label = label_end});
                
                generate_code(node->children[2]);
                
                add_quadruple(LABEL, "", "", (Result){.label = label_end});
            }
            break;
            
        case LOOP:
            // while condition statement_block
            /*  Label:
                t_result = condition
                ifFalse t_result goto Label
                statement_block
                goto Label
            */
            {
                int label_start = new_label();
                int label_end = new_label();

                add_quadruple(LABEL, "", "", (Result){.label = label_start});

                char* condition_result = generate_condition(node->children[1]);
                add_quadruple(IFGOTO, condition_result, "0", (Result){.label = label_end});

                generate_code(node->children[2]);

                add_quadruple(GOTO, "", "", (Result){.label = label_start});

                add_quadruple(LABEL, "", "", (Result){.label = label_end});
            }
            break;
            
        default:
            for (int i = 0; i < node->childCount; i++) {
                generate_code(node->children[i]);
            }
            break;
    }
}

/** @brief 生成表达式的中间代码
 *  @return 表达式结果存储的临时变量
 */
char* generate_expression(ASTNode* node) {
    if (!node) return "";
    
    if (node->childCount == 1) {
        return generate_term(node->children[0]);
    }
    
    char* result = generate_term(node->children[0]);
    
    for (int i = 1; i < node->childCount; i += 2) {
        char* term_result = generate_term(node->children[i+1]);
        char* temp = new_temp();

        add_quadruple(node->children[i]->type.T, result, term_result, (Result){.arg = temp});

        result = temp;
    }

    return result;
}

/** @brief 生成项的中间代码
 *  @return 项结果存储的临时变量
 */
char* generate_term(ASTNode* node) {
    if (!node) return "";
    
    if (node->childCount == 1) {
        return generate_factor(node->children[0]);
    }
    
    char* result = generate_factor(node->children[0]);
    
    for (int i = 1; i < node->childCount; i += 2) {
        char* factor_result = generate_factor(node->children[i+1]);
        char* temp = new_temp();

        add_quadruple(node->children[i]->type.T, result, factor_result, (Result){.arg = temp});
        
        result = temp;
    }

    return result;
}

/** @brief 生成因子的中间代码
 *  @return 因子结果存储的临时变量或直接值
 */
char* generate_factor(ASTNode* node) {
    if (!node) return "";
    
    if (node->childCount == 1) {
        ASTNode* child = node->children[0];
        
        if (child->type.T == ID || child->type.T == NUM)
            return child->value;
    }
    else if (node->childCount == 3 && node->children[0]->type.T == LP) {
        return generate_expression(node->children[1]);
    }

    return "";
}

/** @brief 生成条件的中间代码
 *  @return 条件结果存储的临时变量
 */
char* generate_condition(ASTNode* node) {
    if (!node) return "";
    
    if (node->childCount >= 3) {
        char* left = generate_expression(node->children[0]);
        
        for (int i = 1; i < node->childCount; i += 2) {
            char* right = generate_expression(node->children[i+1]);
            char* temp = new_temp();
            
            add_quadruple(node->children[i]->children[0]->type.T, left, right, (Result){.arg = temp});
            
            left = temp;
        }
        return left;
    }
    
    return "";
}

#ifdef DEBUG_SEMANTIC
void print_quadruples() {
    for(int i = 0; i < quad_count; i++) {
        Quadruple* temp = quadruples+i;
        if(ADD <= temp->op && temp->op <= NEQU) {
            printf("%s = %s %s %s\n", temp->result.arg, temp->arg1, enum2str[temp->op], temp->arg2);
        }
        else if(temp->op == ASSIGN) {
            printf("%s = %s\n", temp->result.arg, temp->arg1);
        }
        else if(temp->op == GOTO) {
            printf("%s %s%d\n", enum2str[GOTO], enum2str[LABEL], temp->result.label);
        }
        else if(temp->op == IFGOTO) {
            printf("%s %s %s %s%d\n", enum2str[temp->op], temp->arg1, enum2str[GOTO], enum2str[LABEL], temp->result.label);
        }
        else if(temp->op == LABEL) {
            printf("%s%d:\n", enum2str[temp->op], temp->result.label);
        }
    }
}
#endif