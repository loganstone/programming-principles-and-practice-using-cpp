// Copyright 2019 Loagan

#ifndef CALCULATOR_INCLUDE_TOKEN_TOKEN_H_
#define CALCULATOR_INCLUDE_TOKEN_TOKEN_H_

#include <cctype>
#include <iostream>
#include <string>

constexpr char kNone = '0';
constexpr char kNumber = '8';
constexpr char kQuit = 'q';
constexpr char kPrint = ';';
constexpr char kPrompt[] = "> ";
constexpr char kResult[] = "= ";

constexpr char kName = 'a';
constexpr char kLet = 'L';
constexpr char kDeclKey[] = "let";

class Token {
 public:
  char kind;
  double value;
  std::string name;
  explicit Token(char ch) : kind{ch} {}
  Token(char ch, double val) : kind{ch}, value{val} {}
  Token(char ch, std::string n) : kind{ch}, name{n} {}
};

class TokenStream {
 public:
  Token get();
  void putback(Token t);
  void ignore(char c);

 private:
  bool is_buffer_full{false};
  Token buffer{kNone};
};

#endif  // CALCULATOR_INCLUDE_TOKEN_TOKEN_H_
