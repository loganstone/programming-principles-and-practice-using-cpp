// Copyright 2019 Loagan

#include <cmath>
#include <stdexcept>
#include <vector>

#include "token/token.h"

class Variable {
 public:
  std::string name;
  double value;
};

TokenStream ts;
std::vector<Variable> var_table;

double get_value(std::string s) {
  for (const Variable& v : var_table) {
    if (v.name == s) {
      return v.value;
    }
  }
  throw std::runtime_error("get_value: undefined variable '" + s + "'");
}

void set_value(std::string s, double d) {
  for (Variable& v : var_table) {
    if (v.name == s) {
      v.value = d;
      return;
    }
  }
  throw std::runtime_error("set_value: undefined variable '" + s + "'");
}

bool is_declared(std::string var) {
  for (const Variable& v : var_table) {
    if (v.name == var) {
      return true;
    }
  }
  return false;
}

double define_name(std::string var, double val) {
  if (is_declared(var)) {
    throw std::runtime_error("define_name: already defined '" + var + "'");
  }
  var_table.push_back(Variable{var, val});
  return val;
}

/* Calculation:
 *  Statement:
 *  Print
 *  Quit
 *  Calculation Statement
 *
 * Statement:
 *  Declaration
 *  Expression
 *
 * Declaration:
 *  "let" Name "=" Expression
 *
 * Print:
 *  ;
 *
 * Quit:
 *  q
 *
 * Expression:
 *  Term
 *  Expression "+" Term
 *  Expression "-" Term
 *
 * Term:
 *  Primary
 *  Term "*" Primary
 *  Term "/" Primary
 *  Term "%" Primary
 *
 * Primary:
 *  Number
 *  "(" Expression ")"
 *  "-" Primary
 *  "+" Primary
 *
 * Number:
 *  floating-point-literal
 */

double expression();

double primary() {
  Token t = ts.get();
  switch (t.kind) {
    case '(': {
      double d = expression();
      t = ts.get();
      if (t.kind != ')') {
        throw std::runtime_error("missing ')'");
      }
      return d;
    }
    case kNumber:
      return t.value;
    case kName:
      return get_value(t.name);
    case '-':
      return -primary();
    case '+':
      return primary();
    default:
      std::string kind{t.kind};
      throw std::runtime_error("missing primary: " + kind);
  }
}

double term() {
  double left = primary();
  Token t = ts.get();
  while (true) {
    switch (t.kind) {
      case '*':
        left *= primary();
        t = ts.get();
        break;
      case '/': {
        double d = primary();
        if (d == 0) {
          throw std::runtime_error("divide by 0");
        }
        left /= d;
        t = ts.get();
        break;
      }
      case '%': {
        double d = primary();
        if (d == 0) {
          throw std::runtime_error("divide by 0");
        }
        left = std::fmod(left, d);
        t = ts.get();
        break;
      }
      default:
        ts.putback(t);
        return left;
    }
  }
}

double expression() {
  double left = term();
  Token t = ts.get();
  while (true) {
    switch (t.kind) {
      case '+':
        left += term();
        t = ts.get();
        break;
      case '-':
        left -= term();
        t = ts.get();
        break;
      default:
        ts.putback(t);
        return left;
    }
  }
}

double declaration() {
  Token t = ts.get();
  if (t.kind != kName) {
    throw std::runtime_error("declaration requires name");
  }
  std::string var_name = t.name;

  Token next_token = ts.get();
  if (next_token.kind != '=') {
    throw std::runtime_error("declaration requires '=': " + var_name);
  }
  double d = expression();
  define_name(var_name, d);
  return d;
}

double statement() {
  Token t = ts.get();
  switch (t.kind) {
    case kLet:
      return declaration();
    default:
      ts.putback(t);
      return expression();
  }
}

void clean_up_mess() { ts.ignore(kPrint); }

void calculate() {
  while (std::cin) {
    try {
      std::cout << kPrompt;
      Token t = ts.get();
      while (t.kind == kPrint) {
        t = ts.get();
      }

      if (t.kind == kQuit) {
        return;
      }

      ts.putback(t);
      std::cout << kResult << statement() << '\n';
    } catch (std::exception& e) {
      std::cerr << e.what() << '\n';
      clean_up_mess();
    }
  }
}

int main() {
  try {
    calculate();
    return 0;
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "exception\n";
    return 2;
  }
}
