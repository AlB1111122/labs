#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *keywords[] = {"auto",     "break",   "case",   "char",     "const",
                          "continue", "default", "do",     "double",   "else",
                          "enum",     "extern",  "float",  "for",      "goto",
                          "if",       "int",     "long",   "register", "return",
                          "short",    "signed",  "sizeof", "static",   "struct",
                          "switch",   "typedef", "union",  "unsigned", "void",
                          "volatile", "while"};
const int nKeywords = 32;

const char *arithmeticOperators[] = {"+", "-", "/", "%", "*"};
const int nArithOp = 5;

const char *relationalOperators[] = {">", "<", "<=", ">=", "!=", "=="};
const int nRelOp = 6;

const char *assignmentOperators[] = {"=", "+=", "-=", "*=", "/="};
const int nAsOp = 5;

const char delimiters[] = {';', '(', ')', '{', '}', '[', ']', ','};
const int nDelim = 8;

int isType(const char *token, const char *typeList[], int typeSz) {
  for (int i = 0; i < typeSz; i++) {
    if (strcmp(token, typeList[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

int isDelimiter(char ch) {
  for (int i = 0; i < nDelim; i++) {
    if (ch == delimiters[i]) {
      return 1;
    }
  }
  return 0;
}

int isIdentifier(char *str) {
  if (!isalpha(str[0]) && str[0] != '_') {
    return 0;
  }
  for (int i = 1; str[i] != '\0'; i++) {
    if (!isalnum(str[i]) && str[i] != '_') {
      return 0;
    }
  }
  return 1;
}

int isNum(char *str) {
  int i = 0;
  int hasDigits = 0;
  int hasDot = 0;

  if (str[i] == '-' || str[i] == '+') {
    i++;
  }

  while (isdigit(str[i])) {
    hasDigits = 1;
    i++;
  }

  if (str[i] == '.') {
    hasDot = 1;
    i++;

    while (isdigit(str[i])) {
      hasDigits = 1;
      i++;
    }
  }

  return hasDigits && str[i] == '\0';
}

void isString(const char *input, int *index) {
  char lexeme[510];
  int idx = 0;
  (*index)++;

  while (input[*index] != '\0' && input[*index] != '"') {
    if (idx < 509) {
      lexeme[idx++] = input[*index];
    }
    (*index)++;
  }

  lexeme[idx] = '\0';
  printf("String Literal: \"%s\"\n", lexeme);
}

void skipSingleLineComment(const char *input, int *index) {
  (*index) += 2;
  while (input[*index] != '\0' && input[*index] != '\n') {
    (*index)++;
  }
}

void skipMultiLineComment(const char *input, int *index) {
  (*index) += 2;
  while (input[*index] != '\0' &&
         !(input[*index] == '*' && input[*index + 1] == '/')) {
    (*index)++;
  }
  if (input[*index] != '\0') {
    (*index) += 2;
  }
}

void lexer(char *input) {
  char lexeme[510];
  int idx = 0;
  int len = strlen(input);
  int inString = 0;

  for (int i = 0; i <= len; i++) {

    if (input[i] == '/' && input[i + 1] == '/') {
      skipSingleLineComment(input, &i);
      continue;
    } else if (input[i] == '/' && input[i + 1] == '*') {
      skipMultiLineComment(input, &i);
      continue;
    }

    if (input[i] == '"') {
      isString(input, &i);
      continue;
    }

    if (isDelimiter(input[i]) || input[i] == '\0' || isspace(input[i])) {
      if (idx > 0) {
        lexeme[idx] = '\0';

        if (isType(lexeme, keywords, nKeywords)) {
          printf("Keyword: %s\n", lexeme);
        } else if (isNum(lexeme)) {
          printf("Number: %s\n", lexeme);
        } else if (isIdentifier(lexeme)) {
          printf("Identifier: %s\n", lexeme);
        } else if (isType(lexeme, relationalOperators, nRelOp)) {
          printf("Operator (relational): %s\n", lexeme);
        } else if (isType(lexeme, arithmeticOperators, nArithOp)) {
          printf("Operator (arithmatic): %s\n", lexeme);
        } else if (isType(lexeme, assignmentOperators, nAsOp)) {
          printf("Operator (asignment): %s\n", lexeme);
        } else {
          printf("Unknown: %s\n", lexeme);
        }

        idx = 0;
      }

      if (isDelimiter(input[i])) {
        printf("Punctuation: %c\n", input[i]);
      }
    } else {
      if (idx < 509) {
        lexeme[idx++] = input[i];
      }
    }
  }
}

int main() {
  FILE *file = fopen("input.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  rewind(file);

  char *input = malloc(length + 1);
  if (input == NULL) {
    perror("Memory allocation failed");
    fclose(file);
    return 1;
  }

  size_t readSize = fread(input, 1, length, file);
  input[readSize] = '\0';

  fclose(file);

  lexer(input);

  free(input);
  return 0;
}