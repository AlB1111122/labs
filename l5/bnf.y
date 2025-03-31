%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <iostream>
using namespace std;

void yyerror(const char *s);
int yylex(void);

vector<string> nonTerminals;
vector<string> terminals;
map<string, vector<string>> productions;  // key = non-terminal, value = list of right-hand sides

// Function to add a production rule
void addProduction(const string& lhs, const string& rhs) {
    productions[lhs].push_back(rhs);
}

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
    printf("Enter BNF rules (use \"::=\" for equals and end line with semicolon):\n");
    yyparse();
    
    cout << "Non-Terminals: ";
    for (const auto& nt : nonTerminals) {
        cout << nt << " ";
    }
    cout << "\nTerminals: ";
    for (const auto& t : terminals) {
        cout << t << " ";
    }
    cout << endl;
    
    cout << "Productions:\n";
    for (const auto& rule : productions) {
        cout << rule.first << " -> ";
        for (const auto& rhs : rule.second) {
            cout << rhs << " ";
        }
        cout << endl;
    }
    
    return 0;
}