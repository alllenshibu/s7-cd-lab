#/bin/bash

lex calculator.l
yacc calculator.y -d

cc lex.yy.c y.tab.c -ll
