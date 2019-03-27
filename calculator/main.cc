// Copyright 2019 Loagan

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "token.h"

TokenStream ts;

/* Statement:
 *  Print
 *  Quit
 *  Expression
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
    case '-':
      return -primary();
    case '+':
      return primary();
    default:
      throw std::runtime_error("missing primary");
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
      std::cout << kResult << expression() << '\n';
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
