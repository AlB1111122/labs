#include <bits/stdc++.h>
using namespace std;

class LL1Parser {
 private:
  map<string, vector<string>> grammar;
  set<string> nonTerminals;
  map<string, set<string>> firstSets;
  map<string, set<string>> followSets;
  map<pair<string, string>, string> parsingTable;
  string startSymbol;

 public:
  LL1Parser(const map<string, vector<string>>& inputGrammar,
            const string& startSymbol) {
    this->grammar = inputGrammar;
    this->startSymbol = startSymbol;
    for (const auto& [key, _] : grammar) {
      nonTerminals.insert(key);
    }
    computeFirstSets();
    computeFollowSets();
    buildParsingTable();
  }

  set<string> computeFirst(const string& symbol) {
    if (nonTerminals.find(symbol) == nonTerminals.end()) {
      return {symbol};
    }

    if (!firstSets[symbol].empty()) {
      return firstSets[symbol];
    }

    for (const auto& prod : grammar[symbol]) {
      stringstream ss(prod);
      string sym;
      vector<string> symbols;
      while (ss >> sym) symbols.push_back(sym);

      for (size_t i = 0; i < symbols.size(); i++) {
        auto firstSym = computeFirst(symbols[i]);
        for (const auto& f : firstSym) {
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
    return firstSets[symbol];
  }

  void computeFirstSets() {
    for (const auto& nt : nonTerminals) {
      computeFirst(nt);
    }
  }

  set<string> computeFollow(const string& symbol) {
    if (!followSets[symbol].empty()) {
      return followSets[symbol];
    }

    if (symbol == startSymbol) {
      followSets[symbol].insert("$");
    }

    for (const auto& [head, prods] : grammar) {
      for (const auto& prod : prods) {
        stringstream ss(prod);
        string sym;
        vector<string> symbols;
        while (ss >> sym) symbols.push_back(sym);

        for (size_t i = 0; i < symbols.size(); i++) {
          if (symbols[i] == symbol) {
            size_t j = i + 1;
            while (j < symbols.size()) {
              auto nextFirst = computeFirst(symbols[j]);
              for (const auto& f : nextFirst) {
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

  void computeFollowSets() {
    for (const auto& nt : nonTerminals) {
      computeFollow(nt);
    }
  }

  void buildParsingTable() {
    for (const auto& [nonTerminal, productions] : grammar) {
      for (const string& production : productions) {
        stringstream ss(production);
        string firstSymbol;
        ss >> firstSymbol;

        set<string> firstSet = computeFirst(firstSymbol);
        for (const string& terminal : firstSet) {
          if (terminal != "ε") {
            parsingTable[{nonTerminal, terminal}] = production;
          }
        }

        if (firstSet.find("ε") != firstSet.end()) {
          for (const string& followSymbol : computeFollow(nonTerminal)) {
            parsingTable[{nonTerminal, followSymbol}] = production;
          }
        }
      }
    }
  }

  bool parse(const string& input) {
    stack<string> parsingStack;
    parsingStack.push("$");
    parsingStack.push(startSymbol);

    string modifiedInput = input + " $";
    stringstream ss(modifiedInput);
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
      } else if (parsingTable.find({top, tokens[index]}) !=
                 parsingTable.end()) {
        string production = parsingTable[{top, tokens[index]}];
        stringstream ss(production);
        vector<string> symbols;
        while (ss >> token) symbols.push_back(token);
        for (int i = symbols.size() - 1; i >= 0; i--) {
          if (symbols[i] != "ε") parsingStack.push(symbols[i]);
        }
      } else {
        return false;
      }
    }
    return false;
  }

  void printParsingTable() {
    set<string> terminals;
    set<string> nonTerminals;

    for (const auto& entry : parsingTable) {
      nonTerminals.insert(entry.first.first);  // row
      terminals.insert(entry.first.second);    // col
    }

    cout << setw(15) << left << " ";
    for (const string& terminal : terminals) {
      cout << setw(15) << left << terminal;
    }
    cout << endl;

    cout << string(15 + terminals.size() * 15, '-') << endl;

    for (const string& nonTerminal : nonTerminals) {
      cout << setw(15) << left << nonTerminal;
      for (const string& terminal : terminals) {
        auto it = parsingTable.find({nonTerminal, terminal});
        if (it != parsingTable.end()) {
          cout << setw(15) << left << it->second;
        } else {
          cout << setw(15) << left << "";
        }
      }
      cout << endl;
    }
  }

  void printFistFollow() {
    cout << "\nFirst sets:" << endl;
    for (auto& nt : nonTerminals) {
      cout << "First(" << nt << ") = { ";
      for (auto& f : firstSets[nt]) {
        cout << f << ", ";
      }
      cout << "}\n";
    }

    cout << "\nFollow sets:" << endl;
    for (auto& nt : nonTerminals) {
      cout << "Follow(" << nt << ") = { ";
      for (auto& f : followSets[nt]) {
        cout << f << ", ";
      }
      cout << "}\n";
    }
  }
};

int main() {
  map<string, vector<string>> grammar = {{"E", {"T E'"}},
                                         {"E'", {"+ T E'", "ε"}},
                                         {"T", {"F T'"}},
                                         {"T'", {"* F T'", "ε"}},
                                         {"F", {"( E )", "id"}}};

  LL1Parser parser(grammar, grammar.begin()->first);
  parser.printParsingTable();
  parser.printFistFollow();

  cout << "Enter input string (space-separated tokens, e.g., 'id + id * id': ";
  string input;
  getline(cin, input);

  if (parser.parse(input)) {
    cout << "Valid input\n";
  } else {
    cout << "Invalid input\n";
  }
  return 0;
}
