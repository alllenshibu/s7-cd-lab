%{
    #include<stdio.h>
    extern int yylex();
    void yyerror(char *s);
    int flag = 0;
%}

%token WHILE COMPARATOR OPERATOR LPAREN RPAREN NUMBER VARIABLE LBRACE RBRACE SEMICOLON ASSIGN
%left OPERATOR COMPARATOR

%%

START   : WHILE LPAREN CONDITION RPAREN LBRACE CONDITION RBRACE { printf("Valid boolean expression\n"); }
        ;

CONDITION  : VARIABLE COMPARATOR VARIABLE 
            | VARIABLE COMPARATOR NUMBER 
            | NUMBER COMPARATOR VARIABLE 
            | NUMBER COMPARATOR NUMBER 
            ;

STATEMENT_LIST : STATEMENT_LIST STATEMENT
               | STATEMENT
               ;

STATEMENT : VARIABLE ASSIGN EXPRESSION SEMICOLON
          ;

EXPRESSION : VARIABLE OPERATOR VARIABLE
           | VARIABLE OPERATOR NUMBER
           | NUMBER OPERATOR VARIABLE
           | NUMBER OPERATOR NUMBER
           | VARIABLE
           | NUMBER
           ;
%%

int main() {
    printf("Enter while loops: ");
    yyparse();  
    return 0;
}

void yyerror(char *s) {
    printf("\nEntered loop is invalid: %s\n\n", s);
}
