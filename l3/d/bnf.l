%{
#include "y.tab.h"
%}

%%
[ \t\n]+          ;  
"<"[a-zA-Z0-9_]+">"   { yylval.str = strdup(yytext);  printf("NONTERMINAL ");return NONTERMINAL;}
"::="             {  printf("COLONEQ "); return COLON_EQUALS; }
";"             {  printf("SEMICOLON "); return SEMICOLON; }
"|"               { printf("PIPE ");return PIPE; }
\"[^\"]+\"        { yylval.str = strdup(yytext);printf("TERMINAL "); return TERMINAL; }
.                     { return yytext[0]; }  // <-- catch-all LAST
%%

int yywrap() { return 1; }