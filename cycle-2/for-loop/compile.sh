#/bin/bash

lex for.l
yacc for.y -d

cc lex.yy.c y.tab.c -ll
