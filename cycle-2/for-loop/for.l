%{
#include "y.tab.h"
%}

%%
"for"       { return FOR; }
"("         { return LPAREN; }
")"         { return RPAREN; }
";"         { return SEMICOLON; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
[0-9]+      {  return CONDITION; }
[a-zA-Z_][a-zA-Z0-9_]* {return IDENTIFIER; }
"="         { return ASSIGN; }
"++"        { return INCREMENT; }
"//".*      { /* ignore comments */ }
[ \t\n]+    { /* ignore whitespace */ }
.           { /* ignore any other character */ }
%%

int yywrap() {
    return 1;
}