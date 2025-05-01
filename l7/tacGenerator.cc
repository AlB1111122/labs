#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 100
#define MAX_STACK 100
#define MAX_LEN 100

int tempCount = 1;

void getTempVar(char* buffer) { snprintf(buffer, MAX_LEN, "t%d", tempCount++); }

typedef struct {
  char* items[MAX_STACK];
  int top;
} Stack;

void initStack(Stack* s) { s->top = -1; }

int isEmpty(Stack* s) { return s->top == -1; }

void push(Stack* s, const char* item) { s->items[++s->top] = strdup(item); }

char* pop(Stack* s) { return s->items[s->top--]; }

char* peek(Stack* s) { return s->items[s->top]; }

int precedence(const char* op) {
  if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
  if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 2;
  return 0;
}

void generateTAC(char* tokens[], int count) {
  Stack operands, operators;
  initStack(&operands);
  initStack(&operators);

  for (int i = 0; i < count; i++) {
    char* token = tokens[i];
    if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
        strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {
      while (!isEmpty(&operators) &&
             precedence(peek(&operators)) >= precedence(token)) {
        char* right = pop(&operands);
        char* left = pop(&operands);
        char* op = pop(&operators);

        char tempVar[MAX_LEN];
        getTempVar(tempVar);

        printf("%s = %s %s %s\n", tempVar, left, op, right);
        push(&operands, tempVar);
      }
      push(&operators, token);
    } else {
      push(&operands, token);
    }
  }

  while (!isEmpty(&operators)) {
    char* right = pop(&operands);
    char* left = pop(&operands);
    char* op = pop(&operators);

    char tempVar[MAX_LEN];
    getTempVar(tempVar);

    printf("%s = %s %s %s\n", tempVar, left, op, right);
    push(&operands, tempVar);
  }

  printf("x = %s\n", pop(&operands));
}

int tokenize(char* expr, char* tokens[]) {
  int count = 0;
  char* token = strtok(expr, " ");
  while (token != NULL) {
    tokens[count++] = token;
    token = strtok(NULL, " ");
  }
  return count;
}

int main() {
  char expr[256];
  char* tokens[MAX_TOKENS];

  printf("Enter the expression : x = ");
  fgets(expr, sizeof(expr), stdin);
  expr[strcspn(expr, "\n")] = 0;

  int count = tokenize(expr, tokens);

  generateTAC(tokens, count);

  return 0;
}