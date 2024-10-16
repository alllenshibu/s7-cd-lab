%{
#include <stdio.h>
#include <string.h>

char name[5];  
int ignore_flag = 0; 

void update_ignore_flag(char *input) {
    if (strstr(input, name) != NULL) {
        ignore_flag = 1;
    } else {
        ignore_flag = 0;
    }
}
%}

%%
.*\n {
    update_ignore_flag(yytext);
    if (!ignore_flag) {
        printf("Valid String: %s", yytext);
    } else {
        printf("Ignoring String: %s", yytext);
    }
}
%%

int main() {
    printf("Enter your name (at least 4 characters): ");
    scanf("%4s", name);
    printf("Ignoring strings that contain the substring: %s\n", name);
    yylex();  
    return 0;
}
