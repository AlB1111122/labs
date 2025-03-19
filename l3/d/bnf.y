%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

ASTNode *createNode(const char *nodeType, const char *value, int numChildren, ...) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->nodeType = strdup(nodeType);
    node->value = value ? strdup(value) : NULL;
    node->numChildren = numChildren;
    if (numChildren > 0) {
        node->children = malloc(sizeof(ASTNode *) * numChildren);
        va_list args;
        va_start(args, numChildren);
        for (int i = 0; i < numChildren; i++) {
            node->children[i] = va_arg(args, ASTNode*);
        }
        va_end(args);
    } else {
        node->children = NULL;
    }
    return node;
}

void printAST(ASTNode *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) 
        printf("  ");
    if (node->value)
        printf("%s: %s\n", node->nodeType, node->value);
    else
        printf("%s\n", node->nodeType);
    for (int i = 0; i < node->numChildren; i++) {
        printAST(node->children[i], indent + 1);
    }
}

ASTNode *astRoot = NULL;

void yyerror(const char *s);
int yylex(void);
%}

%union {
    char *str;
    ASTNode *node;
}

%token <str> NONTERMINAL TERMINAL
%token <str> COLON_EQUALS PIPE SEMICOLON
%left PIPE

%type <node> S Z R E W T D F

%%
S: R Z { 
        $$ = createNode("S", NULL, 2, $1, $2);
        astRoot = $$; 
    }
  ;

Z: R Z { 
        $$ = createNode("Z", NULL, 2, $1, $2);
    }
  | /* empty */ { 
        $$ = createNode("Z", "epsilon", 0);
    }
  ;

R: NONTERMINAL COLON_EQUALS E SEMICOLON { 
        ASTNode *nonterm = createNode("NONTERMINAL", $1, 0);
        ASTNode *colonEq = createNode("COLON_EQUALS", $2, 0);
        ASTNode *semi = createNode("SEMICOLON", $4, 0);
        $$ = createNode("R", NULL, 4, nonterm, colonEq, $3, semi);
    }
  ;

E: T W { 
        $$ = createNode("E", NULL, 2, $1, $2);
    }
  ;

W: PIPE E { 
        /* For the production W -> PIPE E, include a node for the pipe token */
        ASTNode *pipeNode = createNode("PIPE", $1, 0);
        $$ = createNode("W", NULL, 2, pipeNode, $2);
    }
  | /* empty */ { 
        $$ = createNode("W", "epsilon", 0);
    }
  ;

T: F D { 
        $$ = createNode("T", NULL, 2, $1, $2);
    }
  ;

D: F D { 
        $$ = createNode("D", NULL, 2, $1, $2);
    }
  | /* empty */ { 
        $$ = createNode("D", "epsilon", 0);
    }
  ;

F: NONTERMINAL { 
        $$ = createNode("NONTERMINAL", $1, 0);
    }
  | TERMINAL { 
        $$ = createNode("TERMINAL", $1, 0);
    }
  ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "\nError: %s\n", s);
}

int main() {
    printf("Enter BNF rules:\n");
    yyparse();
    printf("\nAST\n");
    printAST(astRoot, 0);
    return 0;
}