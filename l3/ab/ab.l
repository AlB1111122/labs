%{
#include "y.tab.h"
%}

%%

[0-9]+                { return NUMBER; }
[a-zA-Z]([a-zA-Z]|[0-9])*    { return VARIABLE; }
[ \t]+               ; // skip whitespace
\n                   { return '\n'; }
.                    { return yytext[0]; }

%%

int yywrap() { return 1; }