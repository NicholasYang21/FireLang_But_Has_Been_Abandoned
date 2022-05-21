// This file is a part of FireLang.
// Copyright (c) 2022, Ink. All rights reserved.
// License(MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef FIRELANG_AST_HPP
#define FIRELANG_AST_HPP

#include "../fLexer/fire-lexer.hpp"

#include "../../JSON/json.hpp"

#include <utility>
#include <vector>
#include <string>

namespace ast {

extern std::string Types[2];

class AST {
 public:
  enum class TreeType : int {
    Null,
    Primary
  } type;

  flexer::Token curr_node;

  std::vector<AST> children;

  AST() : type{TreeType::Null}, curr_node{flexer::Unknown}, children{} {}

  explicit AST(TreeType tree_type, flexer::Token tok) :
    type{tree_type}, curr_node{std::move(tok)}, children{}
  {}

  explicit AST(TreeType tree_type, flexer::Token tok, std::vector<AST> children) :
    type{tree_type}, curr_node{std::move(tok)}, children{std::move(children)}
  {}

  [[nodiscard]] bool IsLeaf() const;

  [[nodiscard]] std::string ToString() const;
  [[nodiscard]] nlohmann::json ToJSON() const;
};

} // namespace ast

#endif // FIRELANG_AST_HPP
