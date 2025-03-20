#include <bits/stdc++.h>
using namespace std;

map<string, vector<string>> grammar;
vector<string> non_terminals_order;
set<string> non_terminals;
map<string, set<string>> firstSets;
map<string, set<string>> followSets;
string start_symbol;

set<string> compute_first(string symbol) {
  if (non_terminals.find(symbol) == non_terminals.end()) {
    return {symbol};
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
        auto sym_first = compute_first(symbols[i]);
        for (auto &f : sym_first) {
          if (f != "ε") firstSets[symbol].insert(f);
        }
        if (sym_first.find("ε") == sym_first.end()) {
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

set<string> compute_follow(string symbol) {
  if (!followSets[symbol].empty()) {
    return followSets[symbol];
  }

  if (symbol == start_symbol) {
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
            auto first_next = compute_first(symbols[j]);
            for (auto &f : first_next) {
              if (f != "ε") followSets[symbol].insert(f);
            }
            if (first_next.find("ε") != first_next.end()) {
              j++;
            } else {
              break;
            }
          }
          if (j == symbols.size() && head != symbol) {
            auto follow_head = compute_follow(head);
            followSets[symbol].insert(follow_head.begin(), follow_head.end());
          }
        }
      }
    }
  }

  return followSets[symbol];
}

int main() {
  cout << "Enter the grammar productions (empty line to end):\n";
  string line;
  while (getline(cin, line)) {
    if (line.empty()) break;
    auto pos = line.find("->");
    string head = line.substr(0, pos);
    head.erase(remove(head.begin(), head.end(), ' '), head.end());
    if (non_terminals.find(head) == non_terminals.end()) {
      non_terminals.insert(head);
      non_terminals_order.push_back(head);
    }
    string bodies = line.substr(pos + 2);

    stringstream ss(bodies);
    string prod;
    while (getline(ss, prod, '|')) {
      grammar[head].push_back(prod);
    }
  }

  start_symbol = grammar.begin()->first;

  for (auto &nt : non_terminals_order) {
    compute_first(nt);
  }

  for (auto &nt : non_terminals_order) {
    compute_follow(nt);
  }

  cout << "\nFirst sets:" << endl;
  for (auto &nt : non_terminals_order) {
    cout << "First(" << nt << ") = { ";
    for (auto &f : firstSets[nt]) {
      cout << f << ", ";
    }
    cout << "}\n";
  }

  cout << "\nFollow sets:" << endl;
  for (auto &nt : non_terminals_order) {
    cout << "Follow(" << nt << ") = { ";
    for (auto &f : followSets[nt]) {
      cout << f << ", ";
    }
    cout << "}\n";
  }

  return 0;
}
