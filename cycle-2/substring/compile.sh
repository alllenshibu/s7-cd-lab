#/bin/bash

lex substring.l

cc lex.yy.c -ll
