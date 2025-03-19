%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void yyerror(const char *s);
int yylex(void);
%}

%union {
    char *str;
}

%token <str> NONTERMINAL TERMINAL
%token COLON_EQUALS PIPE SEMICOLON
%left PIPE

%%
S: R Z;
Z: R Z | ;

R: NONTERMINAL COLON_EQUALS E SEMICOLON;

E: T W;
W: PIPE E | ;

T: F D;
D: F D | ;

F: NONTERMINAL | TERMINAL;

%%

void yyerror(const char *s) {
    fprintf(stderr, "\nError: %s\n", s);
}

int main() {
    printf("Enter BNF rules (Ctrl+D to finish):\n");
    yyparse();
    return 0;
}