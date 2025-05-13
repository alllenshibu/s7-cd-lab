%{
    #include<stdio.h>
    #include"y.tab.h"
    extern int yylval;
%}

%%
"while"                         return WHILE;
"<"|">"|"<="|">="|"++"          return COMPARATOR;
"+"|"-"|"*"|"/"                 return OPERATOR;
"="                             return ASSIGN;
"("                             return LPAREN;
")"                             return RPAREN;
"{"                             return LBRACE;
"}"                             return RBRACE;
[0-9]+                          return NUMBER;
[a-zA-Z]+                       return VARIABLE;
[ \t\n]                         {};
;                              {return 0;};
.                               ;
%%

int yywrap(){
    return 1;
}
