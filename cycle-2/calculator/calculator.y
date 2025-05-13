%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);

int yylex(void);

int flag = 0;
%}

%token NUMBER

%left '+' '-'
%left '*' '/' '%'
%left '(' ')'

%%

ArithmeticExpression:
    E {
        printf("\nResult = %d\n", $1);
        return 0; 
    }
;

// Expression rules
E:
    E '+' E { $$ = $1 + $3; }
  | E '-' E { $$ = $1 - $3; }
  | E '*' E { $$ = $1 * $3; }
  | E '/' E {
        if ($3 == 0) {
            yyerror("Division by zero");
            YYABORT; 
        }
        $$ = $1 / $3; 
    }
  | E '%' E {
        if ($3 == 0) {
            yyerror("Division by zero in modulus");
            YYABORT; 
        }
        $$ = $1 % $3; 
    }
  | '(' E ')' { $$ = $2; }
  | NUMBER { $$ = $1; }
;

%%

int main() {
    printf("\nEnter Any Arithmetic Expression (Addition, Subtraction, Multiplication, Division, Modulus):\n");
    yyparse();  
        printf("\nEntered arithmetic expression is Valid\n\n");
    return 0;
}

void yyerror(const char *s) {
    printf("\nEntered arithmetic expression is Invalid: %s\n\n", s);
    flag = 1;  
}
