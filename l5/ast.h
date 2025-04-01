#ifndef AST_H
#define AST_H

typedef struct ASTNode {
  char *nodeType;
  char *value;
  struct ASTNode **children;
  int numChildren;
} ASTNode;

ASTNode *createNode(const char *nodeType, const char *value, int numChildren,
                    ...);
void printAST(ASTNode *node, int indent);

#endif