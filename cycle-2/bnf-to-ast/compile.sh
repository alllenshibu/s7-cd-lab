#/bin/bash

lex convert.l
yacc convert.y -d

cc lex.yy.c y.tab.c -ll
