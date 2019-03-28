// Copyright 2019 Loagan

#include "token/token.h"

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
