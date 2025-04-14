%{
#include "y.tab.h"
%}

%%

[0-9]+          { yylval = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]* { yylval = strdup(yytext); return VARIABLE; }  // Variable regex
[ \t]+         { /* Skip whitespace */ }
"="              { return '='; } // Assignment operator
[+\-*/%]         { return yytext[0]; } // Operators
\n               { return 0; } // End of input (newline)
.                { printf("INVALID: Unexpected character '%s'\n", yytext); return YYerror; } // Invalid characters

%%

int yywrap() {
    return 1;
}