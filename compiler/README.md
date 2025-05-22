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


### TODO: semantic analysis
I suppose I've never learnt such things before🤔😰