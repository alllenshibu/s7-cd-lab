%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);
%}


%token  FOR LPAREN RPAREN SEMICOLON IDENTIFIER ASSIGN  CONDITION  INCREMENT LBRACE RBRACE  STATEMENT

%% 

statement:
    for_statement
    ;

for_statement:
    FOR LPAREN for_initialization SEMICOLON for_condition SEMICOLON for_increment RPAREN LBRACE statements RBRACE
    {
        printf("Valid for statement.\n");
    }
    ;

for_initialization:
    IDENTIFIER ASSIGN CONDITION
    ;

for_condition:
    CONDITION
    ;

for_increment:
    IDENTIFIER INCREMENT
    ;

statements:
    STATEMENT
    | statements STATEMENT
    ;

%% 

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    printf("Enter a C for statement:\n");
    yyparse();
    return 0;
}