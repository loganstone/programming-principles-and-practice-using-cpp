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
  void ignore(char c);

 private:
  bool is_buffer_full{false};
  Token buffer;
};

void TokenStream::putback(Token t) {
  if (is_buffer_full) {
    throw std::runtime_error("buffer is full");
  }
  buffer = t;
  is_buffer_full = true;
}

void TokenStream::ignore(char c) {
  if (is_buffer_full && c == buffer.kind) {
    is_buffer_full = false;
    return;
  }

  is_buffer_full = false;
  char ch = 0;
  while (std::cin >> ch) {
    if (ch == c) {
      return;
    }
  }
}

Token TokenStream::get() {
  if (is_buffer_full) {
    is_buffer_full = false;
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
