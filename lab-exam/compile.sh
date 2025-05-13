#/bin/bash

lex lex.l
yacc yacc.y -d

cc lex.yy.c y.tab.c -ll
