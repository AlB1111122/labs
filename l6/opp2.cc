#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <string>
using namespace std;

int i = 0;
string lasthandle;
vector<string> lStack;
vector<string> inputTokens;
map<string, vector<string>> grammar;
set<string> nonTerminals, terminals;
map<pair<string, string>, char> precedenceTable;
string startSymbol;
int top = 0, l;

vector<string> split(const string &str) {
  vector<string> tokens;
  string token;
  for (char ch : str) {
    if (isspace(ch)) {
      if (!token.empty()) {
        tokens.emplace_back(token);
        token.clear();
      }
    } else if (!isalnum(ch) || ch == '$') {
      if (!token.empty()) {
        tokens.emplace_back(token);
        token.clear();
      }
      tokens.emplace_back(
          string(1, ch));  // add operator/parenthases as separate token
    } else {
      token += ch;
    }
  }
  if (!token.empty()) {
    tokens.emplace_back(token);
  }
  return tokens;
}

set<string> firstTerminals(const string &nonTerminal, set<string> &visited) {
  if (visited.count(nonTerminal)) return {};
  visited.insert(nonTerminal);

  set<string> result;
  for (auto &prod : grammar[nonTerminal]) {
    auto symbols = split(prod);
    if (!symbols.empty()) {
      string first = symbols[0];
      if (terminals.count(first)) {
        result.insert(first);
      } else if (nonTerminals.count(first)) {
        auto subFirsts = firstTerminals(first, visited);
        result.insert(subFirsts.begin(), subFirsts.end());
      }
    }
  }
  return result;
}

set<string> lastTerminals(const string &nonTerminal, set<string> &visited) {
  set<string> result;
  if (visited.count(nonTerminal)) return result;
  visited.insert(nonTerminal);

  for (auto &prod : grammar[nonTerminal]) {
    auto symbols = split(prod);
    if (!symbols.empty()) {
      string last = symbols.back();
      if (terminals.count(last)) {
        result.insert(last);
      } else if (nonTerminals.count(last)) {
        auto subLasts = lastTerminals(last, visited);
        result.insert(subLasts.begin(), subLasts.end());
      }
    }
  }
  return result;
}

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
          auto firsts = firstTerminals(b, visited);
          for (const auto &first : firsts) {
            precedenceTable[{a, first}] = '<';
          }
        }

        if (nonTerminals.count(a) && terminals.count(b)) {
          set<string> visited;
          auto lasts = lastTerminals(a, visited);
          for (const auto &last : lasts) {
            precedenceTable[{last, b}] = '>';
          }
        }

        if (nonTerminals.count(a) && nonTerminals.count(b)) {
          set<string> visited;
          auto lasts = lastTerminals(a, visited);
          auto firsts = firstTerminals(b, visited);
          for (const auto &last : lasts) {
            for (const auto &first : firsts) {
              precedenceTable[{last, first}] = '>';
            }
          }
        }
      }
    }
  }

  // Add special $
  for (const auto &t : terminals) {
    precedenceTable[{"$", t}] = '<';
    precedenceTable[{t, "$"}] = '>';
  }
  precedenceTable[{"$", "$"}] = '=';
}

void parseInputString(const string &inputString) {
  stack<char> S;
  string input = inputString + "$";  // Append $ to mark the end of input
  S.push('$');                       // Start with $ symbol in the stack

  cout << "\nParsing input string: " << input << endl;

  int i = 0;  // Input string pointer

  // Print column headers with consistent width
  cout << left << setw(30) << "Stack" << setw(30) << "Input String" << setw(30)
       << "Action" << endl;

  while (true) {
    char top = S.top();       // Get top element from the stack
    char current = input[i];  // Get the current character from the input string

    // Print the stack contents in reverse order (top first)
    cout << "Stack: ";
    stack<char> temp = S;
    string stackContent = "";
    while (!temp.empty()) {
      stackContent = temp.top() + stackContent;
      temp.pop();
    }

    // Print the current stack, input string, and the action
    cout << left << setw(30) << stackContent << setw(30) << input.substr(i)
         << setw(30);

    // Determine the precedence action
    char precedence = precedenceTable[{top, current}];

    if (top == '$' && current == '$') {
      cout << "Parsing completed!" << endl;
      break;
    } else if (precedence == '<') {
      cout << "Shift: " << current << endl;
      S.push(current);
      i++;
    } else if (precedence == '>') {
      cout << "Reduce: " << top << endl;
      S.pop();
    } else if (precedence == '=') {
      cout << "Accepting input" << endl;
      break;
    } else {
      cout << "Error: Invalid\n" << endl;
      break;
    }
  }
}

char getPrecedence(const string &a, const string &b) {
  auto it = precedenceTable.find({a, b});
  if (it != precedenceTable.end()) return it->second;
  return ' ';
}

void shift() { lStack.emplace_back(inputTokens[i++]); }

bool reduce() {
  for (const auto &[head, prods] : grammar) {
    for (auto const &prod : prods) {
      auto symbols = split(prod);
      unsigned long len = symbols.size();
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
          lStack.emplace_back(head);
          lasthandle = prod.c_str();
          return true;
        }
      }
    }
  }
  return false;
}

string findLastTerminal(const vector<string> &stack) {
  if (stack.empty()) return "$";

  for (int idx = stack.size() - 1; idx >= 0; --idx) {
    if (terminals.count(stack[idx]) || stack[idx] == "$") {
      return stack[idx];
    }
  }
  return "$";
}

void displayStack() {
  for (const auto &s : lStack) cout << s;
}

void displayInput() {
  for (size_t j = i; j < inputTokens.size(); ++j) cout << inputTokens[j];
}

void parse() {
  cout << "\nSTACK\tINPUT STRING\tACTION";
  while (i < inputTokens.size()) {
    shift();
    cout << "\n";
    displayStack();
    cout << "\t";
    displayInput();
    cout << "\tShift";

    while (true) {
      string stackTop = findLastTerminal(lStack);
      string currentInput = (i < inputTokens.size()) ? inputTokens[i] : "$";
      char precedence = getPrecedence(stackTop, currentInput);

      if (precedence == '>' && reduce()) {
        cout << "\n";
        displayStack();
        cout << "\t";
        displayInput();
        cout << "\tReduced: E->" << lasthandle;
      } else if ((precedence == '<' || precedence == '=') &&
                 i < inputTokens.size()) {
        shift();
        cout << "\n";
        displayStack();
        cout << "\t";
        displayInput();
        cout << "\tShift";
      } else {
        break;
      }
    }
  }
}

int main() {
  cout << "Enter grammar (space-separated, e.g., E -> E + T | T):\n";
  string line;

  stringstream ss1(
      "E -> E - T | T\nT -> T / F | F\nF -> F ^ G | G\nG -> ( E ) | id | num");
  cout << ss1.str() << endl;

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
      grammar[head].emplace_back(prod);
    }
  }

  startSymbol = grammar.begin()->first;

  for (auto const &[head, prods] : grammar) {
    for (auto const &prod : prods) {
      for (auto token : split(prod)) {
        if (!isupper(token[0]) && token != "ε") terminals.insert(token);
      }
    }
  }

  buildPrecedenceTable();
  printPrecedenceTable();

  string userInput;
  cout << "\nEnter the string:\n";
  // cin >> userInput;
  userInput = "id-num^id";
  cout << userInput << endl;

  inputTokens = split(userInput);
  inputTokens.emplace_back("$");

  lStack.clear();
  lStack.emplace_back("$");

  parse();

  if (lStack.size() == 3 && lStack[0] == "$" && lStack[1] == startSymbol &&
      lStack[2] == "$")
    cout << "\nAccepted;\n";
  else
    cout << "\nNot Accepted;\n";
}