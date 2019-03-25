// Copyright 2019 Loagan
#include <iostream>
#include <stdexcept>

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
    case '=':
    case 'x':
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
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
      return Token{'8', val};
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
    case '8':
      return t.value;
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
    double val = 0;
    while (std::cin) {
      Token t = ts.get();
      if (t.kind == 'x') {
        break;
      }

      if (t.kind == '=') {
        std::cout << "= " << val << '\n';
      } else {
        ts.putback(t);
      }
      val = expression();
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "exception\n";
    return 2;
  }
}
