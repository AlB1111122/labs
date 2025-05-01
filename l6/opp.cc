#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TERMINALS 10
#define MAX_INPUT_LEN 100
#define MAX_STACK_SIZE 100

char terminals[MAX_TERMINALS];
int terminal_count = 0;
char precedence_table[MAX_TERMINALS + 1][MAX_TERMINALS + 1];  // +1 for $
char stack[MAX_STACK_SIZE];
int top = -1;

int get_index(char ch) {
  if (ch == '$') return terminal_count;
  for (int i = 0; i < terminal_count; ++i) {
    if (terminals[i] == ch) return i;
  }
  return -1;
}

void push(char ch) {
  if (top < MAX_STACK_SIZE - 1) stack[++top] = ch;
}

char pop() {
  if (top >= 0) return stack[top--];
  return '\0';
}

char peek() {
  if (top >= 0) return stack[top];
  return '\0';
}

void print_stack() {
  for (int i = top; i >= 0; --i) printf("%c ", stack[i]);
}

void print_precedence_table() {
  printf("\nOperator Precedence Table:\n");
  printf("          ");
  for (int i = 0; i < terminal_count; ++i) printf("%-5c", terminals[i]);
  printf("%-5c\n", '$');

  for (int i = 0; i < 10 + 5 * (terminal_count + 1); ++i) printf("-");
  printf("\n");

  for (int i = 0; i <= terminal_count; ++i) {
    if (i == terminal_count)
      printf("%-10c", '$');
    else
      printf("%-10c", terminals[i]);

    for (int j = 0; j <= terminal_count; ++j) {
      printf("%-5c", precedence_table[i][j]);
    }
    printf("\n");
  }
}

void parse_input_string(char* input) {
  char augmented_input[MAX_INPUT_LEN];
  snprintf(augmented_input, sizeof(augmented_input), "%s$", input);

  top = -1;
  push('$');

  int i = 0;
  printf("\nParsing input string: %s\n", augmented_input);
  printf("\nSTACK\t\tINPUT STRING\tACTION\n");

  while (1) {
    char top_symbol = peek();
    char current = augmented_input[i];

    print_stack();
    printf("\t\t%s\t", &augmented_input[i]);

    if (top_symbol == '$' && current == '$') {
      printf("Parsing completed!\n");
      break;
    }

    int row = get_index(top_symbol);
    int col = get_index(current);

    if (row == -1 || col == -1) {
      printf("Invalid symbol encountered.\n");
      break;
    }

    char relation = precedence_table[row][col];
    if (relation == '<') {
      printf("Shift: %c\n", current);
      push(current);
      i++;
    } else if (relation == '>') {
      printf("Reduce: %c\n", top_symbol);
      pop();
    } else if (relation == '=') {
      printf("Accepted\n");
      break;
    } else {
      printf("Rejected\n");
      break;
    }
  }
}

int main() {
  char terminal_input[MAX_TERMINALS + 1];
  printf("Enter the terminals (single char each, no spaces, $ is reserved):\n");
  scanf("%s", terminal_input);

  if (strchr(terminal_input, '$') != NULL) {
    printf("$ is reserved. Do not include it.\n");
    return 1;
  }

  terminal_count = strlen(terminal_input);
  for (int i = 0; i < terminal_count; ++i) {
    terminals[i] = terminal_input[i];
  }

  printf("Enter the table values (>, <, or =):\n");
  for (int i = 0; i < terminal_count; ++i) {
    for (int j = 0; j < terminal_count; ++j) {
      char rel[2];
      printf("Enter value for %c %c: ", terminals[i], terminals[j]);
      scanf("%s", rel);
      if (rel[0] != '<' && rel[0] != '>' && rel[0] != '=') {
        printf("Invalid input.\n");
        return 1;
      }
      precedence_table[i][j] = rel[0];
    }
  }

  // Fill $ row and column
  for (int i = 0; i < terminal_count; ++i) {
    precedence_table[terminal_count][i] = '<';  // $ vs terminal
    precedence_table[i][terminal_count] = '>';  // terminal vs $
  }
  precedence_table[terminal_count][terminal_count] = '=';  // $ vs $

  print_precedence_table();

  char input_string[MAX_INPUT_LEN];
  printf("\nEnter input string: ");
  scanf("%s", input_string);

  parse_input_string(input_string);

  return 0;
}