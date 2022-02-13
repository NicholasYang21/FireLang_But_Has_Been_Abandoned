// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#ifndef FIRESCRIPT_AST_HPP
#define FIRESCRIPT_AST_HPP

#include "../fLexer/fLexer.hpp"

#include "../fLib/global-functions.hpp"

#include <utility>
#include <vector>

namespace ast {

using fLexer::Token;

class Node {
 public:
  enum { _node = 0, _leaf } nodetype;
  /**
   * The type of this tree (expression or statement):
   *    Block, Statements, Null Statement
   */
  enum { Block, Stmt, Null, Value } stmt_type;

  Token tok;


  std::vector<Node> sub_nodes;

  Node() : nodetype{_leaf}, stmt_type{Null}, tok{fLexer::Unknown} {};

  Node(decltype(nodetype) ty, decltype(stmt_type) tr, Token t) : nodetype{ty}, stmt_type{tr},
                                                                 tok{std::move(t)} { }

  void Print(int depth = 2) const;
};

} // namespace ast

#endif // FIRESCRIPT_AST_HPP
