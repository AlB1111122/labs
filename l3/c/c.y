%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
%}

%token NUMBER

%left '+' '-'
%left '*' '/'

%%

S: E { 
       printf("\nResult: %d\n", $1); 
       return 0; 
     }
;

E: E '+' T   { $$ = $1 + $3; } 
  | E '-' T   { $$ = $1 - $3; }    
  | T         { $$ = $1; } 
;

T: T '*' F   { $$ = $1 * $3; } 
  | T '/' F   { $$ = $1 / $3; }
  | F         { $$ = $1; }  
;

F: '(' E ')' { $$ = $2; } 
  | NUMBER   { $$ = $1; } 
;

%%

void yyerror(const char *s) {
    printf("\nINVALID: %s\n", s); 
}

int main() {
   printf("\nEnter an arithmetic expression:\n");
   yyparse();
   return 0;
}