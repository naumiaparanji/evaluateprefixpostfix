#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "ArgumentManager.h"

template <typename T> class Stack {
private:
  vector<T> array;

public:
  void push(T num) { array.push_back(num); }
  bool isEmpty() { return array.empty(); }

  T pop() {
    if (isEmpty()) {
      cerr << "Stack is empty. Cannot pop" << endl;
      throw invalid_argument("");
    }
    T popped = array.back();
    array.pop_back();
    return popped;
  }

  T peek() {
    if (isEmpty()) {
      cerr << "Empty stack" << endl;
      throw invalid_argument("");
    }
    return array.back();
  }
};

int priority(char c) {
  if (c == '+' || c == '-') {
    return 1;
  } else if (c == '/' || c == '*') {
    return 2;
  } else if (c == '^') {
    return 3;
  } else {
    return 0;
  }
}

string convertPostfix(string exp) {
  Stack<char> s;
  string postfix = "";
  for (int i = 0; i < exp.length(); i++) {
    if (exp[i] == ' ') {
      postfix = postfix + exp[i];
    } else if (exp[i] == '(') {
      s.push(exp[i]);
    } else if (exp[i] == ')') {
      while (!s.isEmpty() && s.peek() != '(') {
        postfix = postfix + s.peek();
        s.pop();
      }
      s.pop();
    } else if (isdigit(exp[i])) {
      while (isdigit(exp[i])) {
        postfix = postfix + exp[i];
        i++;
      }
      i--;
    } else {
      if (!s.isEmpty() && priority(exp[i]) <= priority(s.peek())) {
        postfix = postfix + s.peek();
        s.pop();
      }
      s.push(exp[i]);
    }
  }

  while (!s.isEmpty()) {
    postfix = postfix + s.peek();
    s.pop();
  }

  return postfix;
}

double evaluateInfix(string exp) {
  Stack<double> s;
  double operand1, operand2, result;
  for (int i = 0; i < exp.length(); i++) {
    if (exp[i] == ' ') {
      continue;
    }

    else if (isdigit(exp[i])) {
      double num = 0;
      while (isdigit(exp[i])) {
        num = num * 10 + (double)(exp[i] - '0');
        i++;
      }
      i--;
      s.push(num);
    }

    else {
      operand2 = s.peek(); // first pop
      s.pop();
      operand1 = s.peek(); // second pop
      s.pop();
      if (exp[i] == '*') {
        result = operand2 * operand1;
        s.push(result);
      }

      else if (exp[i] == '/' && operand1 != 0) {
        result = operand2 / operand1;
        s.push(result);
      }

      else if (exp[i] == '+') {
        result = operand2 + operand1;
        s.push(result);
      }

      else if (exp[i] == '-') {
        result = operand2 - operand1;
        s.push(result);
      }

      else if (exp[i] == '^') {
        result = pow(operand2, operand1);
        s.push(result);
      }
    }
  }
  return s.peek();
}

double evaluatePostFix(string exp) {
  Stack<double> s;
  double operand1, operand2, result;
  for (int i = 0; i < exp.length(); i++) {
    if (exp[i] == ' ') {
      continue;
    }

    else if (isdigit(exp[i])) {
      double num = 0;
      while (isdigit(exp[i])) {
        num = num * 10 + (double)(exp[i] - '0');
        i++;
      }
      i--;
      s.push(num);
    }

    else {
      operand2 = s.peek();
      s.pop();
      operand1 = s.peek();
      s.pop();
      if (exp[i] == '*') {
        result = operand1 * operand2;
        s.push(result);
      } else if (exp[i] == '/' && operand2 != 0) {
        result = operand1 / operand2;
        s.push(result);
      } else if (exp[i] == '+') {
        result = operand1 + operand2;
        s.push(result);
      } else if (exp[i] == '-') {
        result = operand1 - operand2;
        s.push(result);
      } else if (exp[i] == '^') {
        result = pow(operand1, operand2);
        s.push(result);
      }
    }
  }
  return s.peek();
}

int main(int argc, char *argv[]) {
  ArgumentManager am(argc, argv);
  const string input = am.get("input");
  const string output = am.get("output");

  ifstream fin(input);
  ofstream fout(output);

  string line, expression, word;

  getline(fin, line);
  stringstream ss(line);
  ss >> word;

  getline(fin, expression);

  if (word == "postfix") {
    double result = evaluatePostFix(expression);
    fout << fixed << setprecision(2) << result << endl;
  } else if (word == "infix") {
    string postfix = convertPostfix(expression);
    double result = evaluatePostFix(postfix);
    fout << fixed << setprecision(2) << result << endl;
  } else if (word == "prefix") {
    string newexpression = "";

    for (int i = 0; i < expression.length(); i++) {
      if (isdigit(expression[i])) {
        int num = 0;
        while (isdigit(expression[i])) {
          num = num * 10 + (int)(expression[i] - '0');
          i++;
        }
        i--;

        string newnum = to_string(num), newNumber = "";
        for (int i = 0; i < newnum.length(); i++) {
          newNumber = newnum[i] + newNumber;
        }
        newexpression = newexpression + newNumber;
      } else {
        newexpression = newexpression + expression[i];
      }
    }

    string newExpression = "";
    for (int i = 0; i < newexpression.length(); i++) {
      newExpression = newexpression[i] + newExpression;
    }

    double result = evaluateInfix(newExpression);
    fout << fixed << setprecision(2) << result << endl;
  }

  fin.close();
  fout.close();
}