#/bin/bash

yacc -d for.y
lex for.l

cc lex.yy.c y.tab.c -ll
