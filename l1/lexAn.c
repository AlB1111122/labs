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

int isInteger(char *str) {
  int i = 0;

  if ((str[0] == '-' || str[0] == '+') && isdigit(str[1])) {
    i++;
  }

  while (isdigit(str[i])) {
    i++;
  }
  return str[i] == '\0';
}

void isString(const char *input, int *index) {
  char lexeme[510];
  int idx = 0;
  (*index)++; // skip past first quote

  while (input[*index] != '\0' && input[*index] != '"') {
    if (idx < 509) {
      lexeme[idx++] = input[*index];
    }
    (*index)++;
  }

  lexeme[idx] = '\0';
  printf("String Literal: \"%s\"\n", lexeme);
}

void lexer(char *input) {
  char lexeme[510];
  int idx = 0;
  int len = strlen(input);
  int inString = 0;

  for (int i = 0; i <= len; i++) {
    if (input[i] == '"') {
      isString(input, &i);
      continue;
    }

    if (isDelimiter(input[i]) || input[i] == '\0' || isspace(input[i])) {
      if (idx > 0) {
        lexeme[idx] = '\0';

        if (isType(lexeme, keywords, nKeywords)) {
          printf("Keyword: %s\n", lexeme);
        } else if (isInteger(lexeme)) {
          printf("Integer: %s\n", lexeme);
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
  char *input = NULL;
  size_t size = 0;
  size_t len = 0;

  printf("Enter your code (end input with an empty line) e.g. int a = 10;\n if "
         "(a >= -5) a = a + 1;\n char str[] = \"Hello, world!\"; :\n");

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), stdin)) {
    if (strcmp(buffer, "\n") == 0)
      break; // Stop on empty line

    size_t buffer_len = strlen(buffer);
    char *new_input =
        realloc(input, len + buffer_len + 1); // +1 for null terminator
    if (!new_input) {
      fprintf(stderr, "Memory allocation error!\n");
      free(input);
      return 1;
    }
    input = new_input;

    memcpy(input + len, buffer, buffer_len);
    len += buffer_len;
    input[len] = '\0'; // Null terminate
  }

  if (input) {
    lexer(input);
    free(input);
  } else {
    printf("No input provided.\n");
  }

  return 0;
}