#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

vector<string> tokenize(const string &line) {
  vector<string> tokens;
  stringstream ss(line);
  string token;
  while (ss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

int main() {
  unordered_map<string, string> constants;
  vector<string> lines;

  cout << "Enter lines of code (end with empty line):\n";

  string line;
  while (getline(cin, line)) {
    if (line.empty()) break;
    lines.push_back(line);
  }

  cout << "\nPropagated code:\n";

  for (const string &code : lines) {
    vector<string> tokens = tokenize(code);

    if (tokens.size() == 4) {
      string op = tokens[0];
      string arg1 = tokens[1];
      string arg2 = tokens[2];
      string result = tokens[3];

      if (constants.find(arg1) != constants.end()) arg1 = constants[arg1];
      if (constants.find(arg2) != constants.end()) arg2 = constants[arg2];

      cout << op << " " << arg1 << " " << arg2 << " " << result << endl;
    } else if (tokens.size() == 3 && tokens[0] == "=") {
      string value = tokens[1];
      string variable = tokens[2];

      bool isConstant = true;
      for (char ch : value) {
        if (!isdigit(ch)) {
          isConstant = false;
          break;
        }
      }

      if (isConstant) {
        constants[variable] = value;
      } else if (constants.find(value) != constants.end()) {
        constants[variable] = constants[value];
      }

      cout << tokens[0] << " " << value << " " << variable << endl;
    }
  }

  return 0;
}
