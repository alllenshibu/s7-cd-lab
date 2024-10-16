#/bin/bash

lex variable.l
yacc variable.y -d

cc lex.yy.c y.tab.c -ll
