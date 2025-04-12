#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <string>
using namespace std;

vector<string> inputTokens;
map<pair<char, char>, char> precedenceTable;
set<char> terminals;

void printPrecedenceTable() {
  cout << "\nOperator Precedence Table:\n";
  cout << setw(10) << left << ' ';
  for (const auto &t : terminals) cout << setw(5) << left << t;
  cout << setw(5) << left << '$';
  cout << "\n" << string(10 + (terminals.size() + 1) * 5, '-') << "\n";

  for (const auto &row : terminals) {
    cout << setw(10) << left << row;
    for (const auto &col : terminals) {
      cout << setw(5) << left << precedenceTable[{row, col}];
    }
    cout << setw(5) << left << precedenceTable[{row, '$'}];
    cout << "\n";
  }

  cout << setw(10) << left << "$";
  for (const auto &col : terminals) {
    cout << setw(5) << left << precedenceTable[{'$', col}];
  }
  cout << setw(5) << left << precedenceTable[{'$', '$'}];
  cout << "\n";
}

void fillOPT() {
  precedenceTable[{'+', '+'}] = '>';
  precedenceTable[{'+', '*'}] = '<';
  precedenceTable[{'+', 'i'}] = '<';

  precedenceTable[{'*', '+'}] = '>';
  precedenceTable[{'*', '*'}] = '>';
  precedenceTable[{'*', 'i'}] = '<';

  precedenceTable[{'i', '+'}] = '>';
  precedenceTable[{'i', '*'}] = '>';
  precedenceTable[{'i', 'i'}] = '=';
}

void parseInputString(const string &inputString) {
  stack<char> S;
  string input = inputString + "$";
  S.push('$');

  cout << "\nParsing input string: " << input << endl;

  cout << "\nSTACK\tINPUT STRING\tACTION\n";

  int i = 0;

  while (true) {
    char top = S.top();
    char current = input[i];

    cout << "Stack: ";
    stack<char> temp = S;
    while (!temp.empty()) {
      cout << temp.top() << " ";
      temp.pop();
    }
    cout << "\t";

    cout << "Input: " << input.substr(i) << "\t";

    if (top == '$' && current == '$') {
      cout << "Parsing completed!" << endl;
      break;
    }
    char precedence = precedenceTable[{top, current}];

    if (precedence == '<') {
      cout << "Shift: " << current << endl;
      S.push(current);
      i++;
    } else if (precedence == '>') {
      cout << "Reduce: " << top << endl;
      S.pop();
    } else if (precedence == '=') {
      cout << "Accepted\n";
      break;
    } else {
      cout << "Rejected\n" << endl;
      break;
    }
  }
}

int main() {
  string terminalsStr = "+*i";
  cout << "Enter the terminals (single char each no spaces $ is reserved):\n";
  //   cin >> terminalsStr;
  if (terminalsStr.find('$') != string::npos) {
    cout << "$ is reserved do not enter it\n";
    return 1;
  }

  for (char &ch : terminalsStr) {
    terminals.emplace(ch);
  }

  //   cout << "Enter the table values (>, <, or =): ";
  //   for (char &chR : terminalsStr) {
  //     for (char &chL : terminalsStr) {
  //       string in;
  //       cout << "\nEnter the table value for " << chR << " " << chL << ": ";
  //       cin >> in;
  //       if (in != ">" && in != "<" && in != "=") {
  //         cout << "\nInvalid input\n";
  //         return 1;
  //       }
  //       precedenceTable[{chR, chL}] = in[0];
  //     }
  //   }
  fillOPT();

  for (const auto &t : terminals) {
    precedenceTable[{'$', t}] = '<';
    precedenceTable[{t, '$'}] = '>';
  }
  precedenceTable[{'$', '$'}] = '=';

  printPrecedenceTable();

  string inputString = "i+i*i";
  cout << "\nEnter input string: ";
  //   cin >> inputString;

  parseInputString(inputString);
}