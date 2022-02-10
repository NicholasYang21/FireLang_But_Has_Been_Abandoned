// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#ifndef FIRESCRIPT_SYMBOL_HPP
#define FIRESCRIPT_SYMBOL_HPP

#include "../fLexer/fLexer.hpp"

#include <map>
#include <string>

using fLexer::Token;

class NormalEnv {
 public:
  NormalEnv* prev = nullptr;
  std::map<std::string, std::pair<std::string, std::string>> table;

  NormalEnv() = delete;

  explicit NormalEnv(NormalEnv* prev);

  void AddSymbol(const std::string&, const std::string&, const std::string&);
};

#endif // FIRESCRIPT_SYMBOL_HPP
