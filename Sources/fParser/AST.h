// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#ifndef FIRESCRIPT_AST_H
#define FIRESCRIPT_AST_H

#include "../fLexer/fLexer.hpp"

#include "../fLib/global-functions.hpp"

#include <vector>

namespace ast {

using fLexer::Token;

class Node {
 public:
  enum tp { _node = 0, _leaf } nodetype;
  Token type;
  std::vector<Node> subs;

  Node() : nodetype{_leaf}, type{fLexer::Unknown} {};

  Node(Token tok, tp type_) : nodetype{type_}, type{tok} {}

  void print(int depth) {
    global::Log(std::cout, "{");
    global::Log(std::cout, R"(  "nodetype": )", 0, false);
    nodetype == _leaf ? global::Log(std::cout, R"("leaf")")
                      : global::Log(std::cout, R"("node")");
    global::Log(std::cout, R"(  "type": )" + type.ToString());
    if (nodetype == _leaf) {}
  }
};

class Arith : public Node {
 public:
  Node left, right;
};

} // namespace ast

#endif // FIRESCRIPT_AST_H
