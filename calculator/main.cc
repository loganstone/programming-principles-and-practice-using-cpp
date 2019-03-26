// Copyright 2019 Loagan
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

constexpr char kNumber = '8';
constexpr char kQuit = 'q';
constexpr char kPrint = ';';
constexpr char kPrompt[] = "> ";
constexpr char kResult[] = "= ";

class Token {
 public:
  char kind;
  double value;
};

class TokenStream {
 public:
  Token get();
  void putback(Token t);

 private:
  bool full{false};
  Token buffer;
};

void TokenStream::putback(Token t) {
  if (full) {
    throw std::runtime_error("buffer is full");
  }
  buffer = t;
  full = true;
}

Token TokenStream::get() {
  if (full) {
    full = false;
    return buffer;
  }
  char ch;
  std::cin >> ch;
  switch (ch) {
    case kQuit:
    case kPrint:
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
      return Token{ch, -1};
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      std::cin.putback(ch);
      double val;
      std::cin >> val;
      return Token{kNumber, val};
    }
    default:
      throw std::runtime_error("invalid token");
  }
}

TokenStream ts;

/* Expression Syntax
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

int main() {
  try {
    while (std::cin) {
      std::cout << kPrompt;
      Token t = ts.get();
      while (t.kind == kPrint) {
        t = ts.get();
      }

      if (t.kind == kQuit) {
        return 0;
      }

      ts.putback(t);
      std::cout << kResult << expression() << '\n';
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "exception\n";
    return 2;
  }
}
