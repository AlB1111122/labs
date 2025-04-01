%{
#include "ast.h"
#include "bnf.tab.h"
%}

%%
[ \t\n]+          ;  
"<"[a-zA-Z0-9_]+">"   { yylval.str = strdup(yytext); return NONTERMINAL;}
"::="             {   return COLON_EQUALS; }
";"             {  return SEMICOLON; }
"|"               { return PIPE; }
\"[^\"]+\"        {     yylval.str = strdup(yytext + 1);  // Skip the first quote
    yylval.str[strlen(yylval.str) - 1] = '\0';  // Remove last quote
    return TERMINAL; 
}
.                     { return yytext[0]; }  // <-- catch-all LAST
%%

int yywrap() { return 1; }