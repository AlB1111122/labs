%{
#include "y.tab.h"
%}

%%

[0-9]+          { yylval = atoi(yytext); return NUMBER; }
[ \t]+         { /* Skip whitespace */ }
[+\-*/%]         { return yytext[0]; } // Operators
\n               { return 0; } // End of input (newline)
.                { printf("INVALID: Unexpected character '%s'\n", yytext); return YYerror; } // Invalid characters

%%

int yywrap() {
    return 1;
}