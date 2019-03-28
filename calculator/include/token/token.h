// Copyright 2019 Loagan

#ifndef CALCULATOR_INCLUDE_TOKEN_TOKEN_H_
#define CALCULATOR_INCLUDE_TOKEN_TOKEN_H_

#include <iostream>

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

#endif  // CALCULATOR_INCLUDE_TOKEN_TOKEN_H_
