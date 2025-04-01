#include <bits/stdc++.h>
using namespace std;

map<string, vector<string>> grammar;
vector<string> nonTermOrder;
set<string> nonTerm;
map<string, set<string>> firstSets;
map<string, set<string>> followSets;
map<pair<string, string>, string> parsingTable;
string startSymbol;

set<string> computeFirst(string symbol) {
  if (nonTerm.find(symbol) == nonTerm.end()) {
    return {symbol};  // Terminal
  }

  if (!firstSets[symbol].empty()) {
    return firstSets[symbol];
  }

  for (auto &prod : grammar[symbol]) {
    if (prod == "ε") {
      firstSets[symbol].insert("ε");
    } else {
      stringstream ss(prod);
      string sym;
      vector<string> symbols;
      while (ss >> sym) symbols.push_back(sym);

      for (size_t i = 0; i < symbols.size(); i++) {
        auto firstSym = computeFirst(symbols[i]);
        for (auto &f : firstSym) {
          if (f != "ε") firstSets[symbol].insert(f);
        }
        if (firstSym.find("ε") == firstSym.end()) {
          break;
        }
        if (i == symbols.size() - 1) {
          firstSets[symbol].insert("ε");
        }
      }
    }
  }
  return firstSets[symbol];
}

set<string> computeFollow(string symbol) {
  if (!followSets[symbol].empty()) {
    return followSets[symbol];
  }

  if (symbol == startSymbol) {
    followSets[symbol].insert("$");
  }

  for (auto &[head, prods] : grammar) {
    for (auto &prod : prods) {
      stringstream ss(prod);
      string sym;
      vector<string> symbols;
      while (ss >> sym) symbols.push_back(sym);

      for (size_t i = 0; i < symbols.size(); i++) {
        if (symbols[i] == symbol) {
          size_t j = i + 1;
          while (j < symbols.size()) {
            auto nextFirst = computeFirst(symbols[j]);
            for (auto &f : nextFirst) {
              if (f != "ε") followSets[symbol].insert(f);
            }
            if (nextFirst.find("ε") != nextFirst.end()) {
              j++;
            } else {
              break;
            }
          }
          if (j == symbols.size() && head != symbol) {
            auto followHead = computeFollow(head);
            followSets[symbol].insert(followHead.begin(), followHead.end());
          }
        }
      }
    }
  }
  return followSets[symbol];
}

void buildParsingTable() {
  for (auto &[nonTerminal, productions] : grammar) {
    for (string production : productions) {
      stringstream ss(production);
      string firstSymbol;
      ss >> firstSymbol;

      set<string> firstSet = computeFirst(firstSymbol);
      for (string terminal : firstSet) {
        if (terminal != "ε") {
          parsingTable[{nonTerminal, terminal}] = production;
        }
      }

      if (firstSet.find("ε") != firstSet.end()) {
        for (string followSymbol : computeFollow(nonTerminal)) {
          parsingTable[{nonTerminal, followSymbol}] = production;
        }
      }
    }
  }
}

bool parse(string input) {
  stack<string> parsingStack;
  parsingStack.push("$");
  parsingStack.push(startSymbol);

  input += " $";  // end marker
  stringstream ss(input);
  vector<string> tokens;
  string token;
  while (ss >> token) tokens.push_back(token);

  int index = 0;
  while (!parsingStack.empty()) {
    string top = parsingStack.top();
    parsingStack.pop();

    if (top == "$" && tokens[index] == "$") return true;

    if (top == tokens[index]) {
      index++;
    } else if (parsingTable.find({top, tokens[index]}) != parsingTable.end()) {
      string production = parsingTable[{top, tokens[index]}];
      stringstream ss(production);
      vector<string> symbols;
      while (ss >> token) symbols.push_back(token);
      for (int i = symbols.size() - 1; i >= 0; i--) {
        if (symbols[i] != "ε") parsingStack.push(symbols[i]);
      }
    } else {
      printf("Can't find: %s\n", tokens[index]);
      return false;
    }
  }
  return false;
}

int main() {
  grammar["E"] = {"T E'"};
  grammar["E'"] = {"+ T E'", "ε"};
  grammar["T"] = {"F T'"};
  grammar["T'"] = {"* F T'", "ε"};
  grammar["F"] = {"( E )", "id"};

  startSymbol = "E";

  for (auto &[key, _] : grammar) {
    nonTerm.insert(key);
  }

  for (auto &nt : nonTerm) computeFirst(nt);
  for (auto &nt : nonTerm) computeFollow(nt);

  buildParsingTable();

  cout << "Enter input string (space-separated tokens, e.g., 'id + id * id', "
          "represent epsilons as ε): ";
  string input;
  getline(cin, input);

  if (parse(input)) {
    cout << "Valid input\n";
  } else {
    cout << "Invalid input\n";
  }

  return 0;
}