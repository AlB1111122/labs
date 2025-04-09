#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <string>
using namespace std;

char *input;
int i = 0;
char lasthandle[6];
vector<string> lStack;
vector<string> inputTokens;
map<string, vector<string>> grammar;
set<string> nonTerminals, terminals;
map<pair<string, string>, char> precedenceTable;
string startSymbol;
int top = 0, l;

// Helper to split string by space
vector<string> split(const string &str) {
  vector<string> tokens;
  string token;
  for (char ch : str) {
    if (isspace(ch)) {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
    } else if (!isalnum(ch) || ch == '$') {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
      tokens.push_back(string(1, ch));  // add operator/paren as separate token
    } else {
      token += ch;
    }
  }
  if (!token.empty()) {
    tokens.push_back(token);
  }
  return tokens;
}

map<string, string> firstCache;
// Compute FIRST terminal of a symbol (for terminals: itself)
string firstTerminal(const string &symbol, set<string> &visited) {
  if (visited.count(symbol)) {
    return "";
  }

  // Mark this symbol as visited
  visited.insert(symbol);

  // If it's a terminal, return it immediately
  if (terminals.count(symbol)) {
    return symbol;
  }

  // Otherwise, process its production rules
  for (auto prod : grammar[symbol]) {
    auto symbols = split(prod);
    if (!symbols.empty()) {
      string first = firstTerminal(symbols[0], visited);
      if (!first.empty()) {
        firstCache[symbol] = first;
        return first;
      }
    }
  }

  // If no terminal found, return an empty string
  return "";
}

// Compute LAST terminal of a symbol
string lastTerminal(const string &symbol) {
  if (terminals.count(symbol)) return symbol;
  for (auto prod : grammar[symbol]) {
    auto symbols = split(prod);
    if (!symbols.empty()) {
      string last = lastTerminal(symbols.back());
      if (!last.empty()) return last;
    }
  }
  return "";
}

// Build operator precedence table
void buildPrecedenceTable() {
  if (grammar.empty()) {
    cout << "Grammar is empty!\n";
    return;
  }

  for (auto &[head, prods] : grammar) {
    for (auto prod : prods) {
      auto symbols = split(prod);
      for (size_t i = 0; i + 1 < symbols.size(); ++i) {
        string a = symbols[i], b = symbols[i + 1];
        if (terminals.count(a) && terminals.count(b)) {
          precedenceTable[{a, b}] = '=';
        }

        if (terminals.count(a) && nonTerminals.count(b)) {
          set<string> visited;
          string first = firstTerminal(b, visited);
          if (!first.empty()) precedenceTable[{a, first}] = '<';
        }

        if (nonTerminals.count(a) && terminals.count(b)) {
          string last = lastTerminal(a);
          if (!last.empty()) precedenceTable[{last, b}] = '>';
        }

        if (nonTerminals.count(a) && nonTerminals.count(b)) {
          string last = lastTerminal(a);
          set<string> visited;
          string first = firstTerminal(b, visited);
          if (!last.empty() && !first.empty())
            precedenceTable[{last, first}] = '>';
        }
      }
    }
  }

  // Add special symbols
  for (const auto &t : terminals) {
    precedenceTable[{"$", t}] = '<';
    precedenceTable[{t, "$"}] = '>';
  }
  precedenceTable[{"$", "$"}] = '=';
}

// Print the operator precedence table
void printPrecedenceTable() {
  cout << "\nOperator Precedence Table:\n";
  cout << setw(10) << left << " ";
  for (const auto &t : terminals) cout << setw(5) << left << t;
  cout << setw(5) << left << "$";
  cout << "\n" << string(10 + (terminals.size() + 1) * 5, '-') << "\n";

  for (const auto &row : terminals) {
    cout << setw(10) << left << row;
    for (const auto &col : terminals) {
      cout << setw(5) << left << precedenceTable[{row, col}];
    }
    cout << setw(5) << left << precedenceTable[{row, "$"}];
    cout << "\n";
  }

  // Row for $
  cout << setw(10) << left << "$";
  for (const auto &col : terminals) {
    cout << setw(5) << left << precedenceTable[{"$", col}];
  }
  cout << setw(5) << left << precedenceTable[{"$", "$"}];
  cout << "\n";
}

// Get precedence relation
char getPrecedence(const string &a, const string &b) {
  auto it = precedenceTable.find({a, b});
  if (it != precedenceTable.end()) return it->second;
  return ' ';
}

void shift() { lStack.push_back(inputTokens[i++]); }

bool reduce() {
  for (const auto &[head, prods] : grammar) {
    for (auto prod : prods) {
      auto symbols = split(prod);
      int len = symbols.size();
      if (len <= lStack.size()) {
        bool match = true;
        for (int t = 0; t < len; ++t) {
          if (lStack[lStack.size() - len + t] != symbols[t]) {
            match = false;
            break;
          }
        }
        if (match) {
          lStack.erase(lStack.end() - len, lStack.end());
          lStack.push_back(head);
          strcpy(lasthandle, prod.c_str());
          return true;  // successful reduction
        }
      }
    }
  }
  return false;
}
string findLastTerminal(const vector<string> &stack) {
  for (int idx = stack.size() - 1; idx >= 0; --idx) {
    if (terminals.count(stack[idx]) || stack[idx] == "$") return stack[idx];
  }
  return "$";  // fallback
}

// Precedence check function (decides shift or reduce based on precedence table)
void dispstack() {
  for (const auto &s : lStack) cout << s;
}

void dispinput() {
  for (size_t j = i; j < inputTokens.size(); ++j) cout << inputTokens[j];
}

int main() {
  cout << "Enter grammar (space-separated, e.g., E -> E + T | T):\n";
  string line;

  stringstream ss1("E -> i | E*E | E+E | (E) | E^E");

  while (getline(ss1, line)) {
    if (line.empty()) break;
    auto pos = line.find("->");
    if (pos == string::npos) continue;
    string head = line.substr(0, pos);
    head.erase(remove(head.begin(), head.end(), ' '), head.end());
    nonTerminals.insert(head);
    string bodies = line.substr(pos + 2);
    stringstream ss(bodies);
    string prod;
    while (getline(ss, prod, '|')) {
      grammar[head].push_back(prod);
    }
  }

  startSymbol = grammar.begin()->first;

  // Extract terminals
  for (auto &[head, prods] : grammar) {
    for (auto prod : prods) {
      for (auto token : split(prod)) {
        if (!isupper(token[0]) && token != "ε") terminals.insert(token);
      }
    }
  }

  buildPrecedenceTable();
  printPrecedenceTable();

  int j;

  string userInput;
  cout << "\nEnter the string:\n";
  // cin >> userInput;
  userInput = "i+i^i";

  inputTokens = split(userInput);
  inputTokens.push_back("$");

  lStack.clear();
  lStack.push_back("$");

  cout << "\nSTACK\tINPUT\tACTION";

  while (i < inputTokens.size()) {
    shift();
    cout << "\n";
    dispstack();
    cout << "\t";
    dispinput();
    cout << "\tShift";

    while (true) {
      string stackTop = findLastTerminal(lStack);
      string currentInput = (i < inputTokens.size()) ? inputTokens[i] : "$";
      char precedence = getPrecedence(stackTop, currentInput);

      if (precedence == '>' && reduce()) {
        cout << "\n";
        dispstack();
        cout << "\t";
        dispinput();
        cout << "\tReduced: E->" << lasthandle;
      } else if ((precedence == '<' || precedence == '=') &&
                 i < inputTokens.size()) {
        shift();
        cout << "\n";
        dispstack();
        cout << "\t";
        dispinput();
        cout << "\tShift";
      } else {
        break;
      }
    }
  }

  if (lStack.size() == 3 && lStack[0] == "$" && lStack[1] == startSymbol &&
      lStack[2] == "$")
    cout << "\nAccepted;\n";
  else
    cout << "\nNot Accepted;\n";
}