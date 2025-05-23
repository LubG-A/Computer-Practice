# A simple compiler
### lexical analysis
a lexer that reads in letters and generates them to tokens stored in list.

The lexer mainly based on function `readToken()` is hard coded, I am seeking for ideas to make it more flexible.

### syntax analysis
a parser analyzing the tokens and builds up AST

using LL(1) grammar

This part is well designed, you can soon understand how it's working and try design grammar of your own!

2025.5.22: Now the program is programmed using grammar as below

|语法|HEAD集|
|:-|:-|
|\<程序\>→begin \<语句串\> end|begin|
|\<语句串\>→\<语句\>{\<语句\>}|ID,if,while|
|\<语句\>→\<赋值语句\>\|\<条件语句\>\|\<循环语句\>|ID,if,while|
|\<赋值语句\>→ ID :=\<表达式\>;|ID|
|\<表达式\>→\<项\>{+\<项\>\|-\<项\>}|ID,NUM,(|
|\<项\>→\<因子\>{*\<因子\>\|/\<因子\>}|ID,NUM,(|
|\<因子\>→ID\|NUM\|(\<表达式\>)|ID,NUM,(|
|\<条件语句\>→ if \<逻辑运算\> then \<语句串\> end|if|
|\<循环语句\>→ while \<逻辑运算\> do \<语句串\> end|while|
|\<逻辑运算\>→\<表达式\>{\<逻辑运算符\>\<表达式\>}|ID,NUM,(|
|\<逻辑运算符\>→\<\|\<=\|=\|\>=\|\>\|\<\>|\<,\<=,=,\>=,\>,\<\>|


2025.5.23: Stupid computer practice task4 is requiring a new grammar different from task2, whoever designed this experiment is definitely lazy and cruel
|grammar|HEAD set|
|:-|:-|
|\<program\>→main() \<statement_block\>|main()|
|\<statement_block\>→'{' \<statement_list\> '}'|'{'|
|\<statement_list\>→\<statement\>{\<statement\>}|ID,if,while|
|\<statement\>→\<assignment\>\|\<if_statement\>\|\<loop_statement\>|ID,if,while|
|\<assignment\>→ ID :=\<expression\>;|ID|
|\<expression\>→\<term\>{+\<term\>\|-\<term\>}|ID,NUM,(|
|\<term\>→\<factor\>{*\<factor\>\|/\<factor\>}|ID,NUM,(|
|\<factor\>→ID\|NUM\|(\<expression\>)|ID,NUM,(|
|\<if_statement\>→ if \<condition\> \<statement_block\>|if|
|\<loop_statement\>→ while \<condition\> \<statement_block\>|while|
|\<condition\>→\<expression\>{\<condition_op\>\<expression\>}|ID,NUM,(|
|\<condition_op\>→\<\|\<=\|==\|\>=\|\>\|\<\>|\<,\<=,=,\>=,\>,!=|

### semantic analysis
I suppose I've never learnt such things before🤔😰

This part is working just like parser, I wrote process functions for each non-terminal symbols. It seems to be working out fine.

### how to use
use `make` to compile, the bianry executable will be under directory bin/

you can use arguments `debug_lexer` `debug_parser` and `debug_semantic` to print the output of each components.

It'll be easy to understand if you take a look at the Makefile

### compiler tests
#### Test Case 1

Input
```sh
main() {
        a := 114;
        b := 514;
        while a < b {
                if a + b == 1919 {
                        a := 810;
                        b := 810;
                }
                a := a + 1;
        }
}
```
Output-debug_semantic:
```sh
a = 114
b = 514
L1:
t1 = a < b
ifFalse t1 goto L2
t2 = a + b
t3 = t2 == 1919
ifFalse t3 goto L3
a = 810
b = 810
L3:
t4 = a + 1
a = t4
goto L1
L2:
```

#### Test Case2

Input
```sh
main() {
        uglyVar:=
uglyInput;
while uglyVar                           ==
uglyInput
{uglyVar:=uglyInput-uglyVar;while uglyVar != 10{
uglyVar:=
uglyVar                 +1;
}
}}
```
Output-debug_parser

This is actually an AST printed out

```sh
program:
    start: main
    statement_block:
        lb: {
        statement_list:
            statement:
                assignment:
                    identifier: uglyVar
                    assign: :=
                    expression:
                        term:
                            factor:
                                identifier: uglyInput
                    semi: ;
            statement:
                loop:
                    while: while
                    condition:
                        expression:
                            term:
                                factor:
                                    identifier: uglyVar
                        condition_op:
                            eq: ==
                        expression:
                            term:
                                factor:
                                    identifier: uglyInput
                    statement_block:
                        lb: {
                        statement_list:
                            statement:
                                assignment:
                                    identifier: uglyVar
                                    assign: :=
                                    expression:
                                        term:
                                            factor:
                                                identifier: uglyInput
                                        sub: -
                                        term:
                                            factor:
                                                identifier: uglyVar
                                    semi: ;
                            statement:
                                loop:
                                    while: while
                                    condition:
                                        expression:
                                            term:
                                                factor:
                                                    identifier: uglyVar
                                        condition_op:
                                            nequ: !=
                                        expression:
                                            term:
                                                factor:
                                                    number: 10
                                    statement_block:
                                        lb: {
                                        statement_list:
                                            statement:
                                                assignment:
                                                    identifier: uglyVar
                                                    assign: :=
                                                    expression:
                                                        term:
                                                            factor:
                                                                identifier: uglyVar
                                                        add: +
                                                        term:
                                                            factor:
                                                                number: 1
                                                    semi: ;
                                        rb: }
                        rb: }
        rb: }
```