#include <cstring>
#include <iostream>
using namespace std;

const int MAX_NON_TERMINALS = 100;
const int MAX_PRODUCTIONS = 100;
const int MAX_SYMBOLS = 100;
const int MAX_LENGTH = 100;

char non_terminals[MAX_NON_TERMINALS][MAX_LENGTH];
int non_terminal_count = 0;

char grammar_heads[MAX_NON_TERMINALS][MAX_LENGTH];
char grammar_bodies[MAX_NON_TERMINALS][MAX_PRODUCTIONS][MAX_LENGTH];
int production_count[MAX_NON_TERMINALS];

char firstSets[MAX_NON_TERMINALS][MAX_SYMBOLS][MAX_LENGTH];
int firstCount[MAX_NON_TERMINALS];

char followSets[MAX_NON_TERMINALS][MAX_SYMBOLS][MAX_LENGTH];
int followCount[MAX_NON_TERMINALS];

char start_symbol[MAX_LENGTH];

int find_non_terminal_index(const char *symbol) {
  for (int i = 0; i < non_terminal_count; ++i) {
    if (strcmp(non_terminals[i], symbol) == 0) return i;
  }
  return -1;
}

bool in_set(char set[MAX_SYMBOLS][MAX_LENGTH], int count, const char *symbol) {
  for (int i = 0; i < count; ++i) {
    if (strcmp(set[i], symbol) == 0) return true;
  }
  return false;
}

void add_to_set(char set[MAX_SYMBOLS][MAX_LENGTH], int &count,
                const char *symbol) {
  if (!in_set(set, count, symbol)) {
    strcpy(set[count++], symbol);
  }
}

void split(const char *str, char tokens[MAX_SYMBOLS][MAX_LENGTH],
           int &token_count) {
  token_count = 0;
  const char *start = str;
  while (*start) {
    while (*start == ' ') ++start;
    if (*start == '\0') break;
    char *end = (char *)start;
    while (*end && *end != ' ') ++end;
    strncpy(tokens[token_count], start, end - start);
    tokens[token_count][end - start] = '\0';
    ++token_count;
    start = end;
  }
}

void compute_first(const char *symbol);

void compute_first_recursive(int nt_index) {
  for (int i = 0; i < production_count[nt_index]; ++i) {
    char symbols[MAX_SYMBOLS][MAX_LENGTH];
    int sym_count;
    split(grammar_bodies[nt_index][i], symbols, sym_count);

    if (sym_count == 1 && strcmp(symbols[0], "ε") == 0) {
      add_to_set(firstSets[nt_index], firstCount[nt_index], "ε");
    } else {
      for (int j = 0; j < sym_count; ++j) {
        compute_first(symbols[j]);
        int idx = find_non_terminal_index(symbols[j]);
        if (idx == -1) {
          add_to_set(firstSets[nt_index], firstCount[nt_index], symbols[j]);
          break;
        } else {
          for (int k = 0; k < firstCount[idx]; ++k) {
            if (strcmp(firstSets[idx][k], "ε") != 0)
              add_to_set(firstSets[nt_index], firstCount[nt_index],
                         firstSets[idx][k]);
          }
          if (!in_set(firstSets[idx], firstCount[idx], "ε")) break;
        }
        if (j == sym_count - 1) {
          add_to_set(firstSets[nt_index], firstCount[nt_index], "ε");
        }
      }
    }
  }
}

void compute_first(const char *symbol) {
  int idx = find_non_terminal_index(symbol);
  if (idx == -1) return;

  if (firstCount[idx] > 0) return;

  compute_first_recursive(idx);
}

void compute_follow(const char *symbol);

void compute_follow_recursive(int nt_index) {
  for (int i = 0; i < non_terminal_count; ++i) {
    for (int j = 0; j < production_count[i]; ++j) {
      char symbols[MAX_SYMBOLS][MAX_LENGTH];
      int sym_count;
      split(grammar_bodies[i][j], symbols, sym_count);
      for (int k = 0; k < sym_count; ++k) {
        if (strcmp(symbols[k], non_terminals[nt_index]) == 0) {
          int l = k + 1;
          while (l < sym_count) {
            compute_first(symbols[l]);
            int idx = find_non_terminal_index(symbols[l]);
            if (idx == -1) {
              add_to_set(followSets[nt_index], followCount[nt_index],
                         symbols[l]);
              break;
            } else {
              for (int m = 0; m < firstCount[idx]; ++m) {
                if (strcmp(firstSets[idx][m], "ε") != 0)
                  add_to_set(followSets[nt_index], followCount[nt_index],
                             firstSets[idx][m]);
              }
              if (!in_set(firstSets[idx], firstCount[idx], "ε")) break;
            }
            ++l;
          }
          if (l == sym_count &&
              strcmp(non_terminals[i], non_terminals[nt_index]) != 0) {
            compute_follow(non_terminals[i]);
            for (int m = 0; m < followCount[i]; ++m) {
              add_to_set(followSets[nt_index], followCount[nt_index],
                         followSets[i][m]);
            }
          }
        }
      }
    }
  }
}

void compute_follow(const char *symbol) {
  int idx = find_non_terminal_index(symbol);
  if (idx == -1) return;
  if (followCount[idx] > 0) return;
  if (strcmp(symbol, start_symbol) == 0) {
    add_to_set(followSets[idx], followCount[idx], "$");
  }
  compute_follow_recursive(idx);
}

int main() {
  cout << "Enter the grammar productions (empty line to end):\n";
  char line[1000];
  while (true) {
    cin.getline(line, sizeof(line));
    if (strlen(line) == 0) break;

    char *arrow = strstr(line, "->");
    if (!arrow) continue;

    char head[MAX_LENGTH];
    strncpy(head, line, arrow - line);
    head[arrow - line] = '\0';
    char *head_trimmed = strtok(head, " ");

    int idx = find_non_terminal_index(head_trimmed);
    if (idx == -1) {
      strcpy(non_terminals[non_terminal_count], head_trimmed);
      idx = non_terminal_count++;
    }

    strcpy(grammar_heads[idx], head_trimmed);

    char *bodies = arrow + 2;
    char *prod = strtok(bodies, "|");
    while (prod) {
      while (*prod == ' ') ++prod;
      strcpy(grammar_bodies[idx][production_count[idx]++], prod);
      prod = strtok(nullptr, "|");
    }
  }

  strcpy(start_symbol, grammar_heads[0]);

  for (int i = 0; i < non_terminal_count; ++i) {
    compute_first(non_terminals[i]);
  }

  for (int i = 0; i < non_terminal_count; ++i) {
    compute_follow(non_terminals[i]);
  }

  cout << "\nFirst sets:\n";
  for (int i = 0; i < non_terminal_count; ++i) {
    cout << "First(" << non_terminals[i] << ") = { ";
    for (int j = 0; j < firstCount[i]; ++j) {
      cout << firstSets[i][j];
      if (j < firstCount[i] - 1) cout << ", ";
    }
    cout << " }\n";
  }

  cout << "\nFollow sets:\n";
  for (int i = 0; i < non_terminal_count; ++i) {
    cout << "Follow(" << non_terminals[i] << ") = { ";
    for (int j = 0; j < followCount[i]; ++j) {
      cout << followSets[i][j];
      if (j < followCount[i] - 1) cout << ", ";
    }
    cout << " }\n";
  }

  return 0;
}