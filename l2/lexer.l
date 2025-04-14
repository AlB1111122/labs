%{
#include <stdio.h>
%}

%%
auto | break | case | char | const | continue | default | do | double | else |
enum | extern | float | for | goto | if | int | long | register | return | 
short | signed | sizeof | static | struct | switch | typedef | union | unsigned |
void | volatile | while { printf("Keyword: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]* { printf("Identifier: %s\n", yytext); }
[0-9]+ { printf("Number: %s\n", yytext); }
[0-9]+\.[0-9]+([eE][-+]?[0-9]+)? { printf("Float: %s\n", yytext); }
[\+\-\*/%] { printf("Operator (arithmatic): %s\n", yytext); }
[\>\<\>=\<=\!=\==] { printf("Operator (relational): %s\n", yytext); }
[+\-*/]?= { printf("Operator (asignment): %s\n", yytext); }
[\;\(\)\{\}\[\]\,] { printf("Punctuation: %s\n", yytext); }
\"([^\"\\]|\\.)*\" { printf("String literal: %s\n", yytext); }
"//".* { printf("Single-line comment: %s\n", yytext); } 
\/\*[^*]*\*+([^/*][^*]*\*+)*\/ { printf("Multi-line comment: %s\n", yytext); } //multi-line comments
[ \t\n] ;
"#".* { printf("Preprocessor directive: %s\n", yytext); } 
. { printf("Unknown character: %s\n", yytext); }
%%
int main(int argc, char **argv) {
  FILE *file;
  file = fopen(argv[1], "r");
  if (!file) {
    printf("could not open the file %s", argv[1]);
    exit(0);
  }
  yyin = file;
  yylex();
  printf("\n");
  return (0);
}
int yywrap() { return (1); }
