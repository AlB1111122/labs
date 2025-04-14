%{
#include<stdio.h>
%}

%token NUMBER
%token VARIABLE

%left '*' '/' '%'
%left '+' '-'

%%

S: VARIABLE '=' E '\n' {
       printf("\nVALID\n");
       return 0;
     }
 ;

E: E '+' E 
 | E '-' E 
 | E '*' E 
 | E '/' E 
 | E '%' E
 | NUMBER 
 | VARIABLE
 ;

%%

void main() { 
   printf("\nEnter valid arithmetic expression\n");
   yyparse();
}

void yyerror() {
   printf("\nINVALID\n");
}