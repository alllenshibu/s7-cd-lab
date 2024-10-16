%{
    #include <stdio.h>

    int yylex();

    void yyerror(const char *s);

    int valid = 1; 
%}

%token identifier  

%%

start : identifier  
      ;

%%

void yyerror(const char *s) {
    printf("\n%s\n", s); 
    valid = 0; 
}

int main() {
    printf("\nEnter a name to be tested for identifier: ");
    yyparse();  
    if (valid) {
        printf("\nIt is a valid identifier!\n");
    }
    return 0;
}
