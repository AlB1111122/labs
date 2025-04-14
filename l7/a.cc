#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;

int tempCount = 1;

string getTempVar() { return "t" + to_string(tempCount++); }

void generateTAC(vector<string>& expression) {
  stack<string> operands;
  stack<string> operators;
  map<string, string> precedence = {
      {"+", "1"}, {"-", "1"}, {"*", "2"}, {"/", "2"}};

  for (string token : expression) {
    if (token == "+" || token == "-" || token == "*" || token == "/") {
      while (!operators.empty() &&
             precedence[operators.top()] >= precedence[token]) {
        string right = operands.top();
        operands.pop();
        string left = operands.top();
        operands.pop();
        string op = operators.top();
        operators.pop();

        string tempVar = getTempVar();
        cout << tempVar << " = " << left << " " << op << " " << right << endl;
        operands.push(tempVar);
      }
      operators.push(token);
    } else {
      operands.push(token);
    }
  }

  while (!operators.empty()) {
    string right = operands.top();
    operands.pop();
    string left = operands.top();
    operands.pop();
    string op = operators.top();
    operators.pop();

    string tempVar = getTempVar();
    cout << tempVar << " = " << left << " " << op << " " << right << endl;
    operands.push(tempVar);
  }

  cout << "result = " << operands.top() << endl;
}

vector<string> tokenize(string expression) {
  vector<string> tokens;
  stringstream ss(expression);
  string token;

  while (ss >> token) {
    tokens.push_back(token);
  }

  return tokens;
}

int main() {
  cout << "Enter the expression : x=";
  string expr;
  getline(cin, expr);
  cout << endl;

  vector<string> tokens = tokenize(expr);

  generateTAC(tokens);

  return 0;
}