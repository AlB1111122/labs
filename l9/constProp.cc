#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_TOKENS 4
#define MAX_VARIABLES 100
#define MAX_STRING_LENGTH 100

typedef struct {
  char variable[MAX_STRING_LENGTH];
  char value[MAX_STRING_LENGTH];
} Variable;

Variable constants[MAX_VARIABLES];
int constant_count = 0;

int tokenize(char* line, char* tokens[]) {
  int count = 0;
  char* token = strtok(line, " ");
  while (token != NULL) {
    tokens[count++] = token;
    token = strtok(NULL, " ");
  }
  return count;
}

int isConstant(char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (!isdigit(str[i])) {
      return 0;
    }
  }
  return 1;
}

char* getConstantValue(char* variable) {
  for (int i = 0; i < constant_count; i++) {
    if (strcmp(constants[i].variable, variable) == 0) {
      return constants[i].value;
    }
  }
  return NULL;
}

int main() {
  char* lines[MAX_LINES];
  char line[MAX_STRING_LENGTH];
  int line_count = 0;

  printf(
      "Enter lines of code (end with empty line) e.g. = 3 a\n+ a b t1\n+ a c "
      "t2\n+ t1 t2 t3 :\n");

  while (fgets(line, sizeof(line), stdin)) {
    line[strcspn(line, "\n")] = 0;
    if (strlen(line) == 0) {
      break;
    }
    lines[line_count] = strdup(line);
    line_count++;
  }

  printf("\nPropagated code:\n");

  for (int i = 0; i < line_count; i++) {
    char* tokens[MAX_TOKENS];
    int token_count = tokenize(lines[i], tokens);

    if (token_count == 4) {
      char* op = tokens[0];
      char* arg1 = tokens[1];
      char* arg2 = tokens[2];
      char* result = tokens[3];

      if (getConstantValue(arg1)) arg1 = getConstantValue(arg1);
      if (getConstantValue(arg2)) arg2 = getConstantValue(arg2);

      printf("%s %s %s %s\n", op, arg1, arg2, result);
    } else if (token_count == 3 && strcmp(tokens[0], "=") == 0) {
      char* value = tokens[1];
      char* variable = tokens[2];

      if (isConstant(value)) {
        strcpy(constants[constant_count].variable, variable);
        strcpy(constants[constant_count].value, value);
        constant_count++;
        printf("%s %s %s\n", tokens[0], value, variable);
      } else {
        char* resolved_value = getConstantValue(value);
        if (resolved_value) {
          strcpy(constants[constant_count].variable, variable);
          strcpy(constants[constant_count].value, resolved_value);
          constant_count++;
          printf("%s %s %s\n", tokens[0], resolved_value, variable);
        }
      }
    }
  }

  for (int i = 0; i < line_count; i++) {
    free(lines[i]);
  }

  return 0;
}