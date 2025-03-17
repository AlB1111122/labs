%{
#include "y.tab.h"
%}

%%

[0-9]+ { yylval = atoi(yytext); return NUMBER; }
[a-zA-Z]([a-zA-Z]|[0-9])* { yylval = strdup(yytext); return VARIABLE; }
[ \t]+           ; 
\n { return 0; } 
. { return yytext[0]; }

%%
int yywrap() { return 1; }