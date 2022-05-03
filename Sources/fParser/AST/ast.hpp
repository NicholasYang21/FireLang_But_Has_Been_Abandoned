// This file is a part of FireLang.
// Copyright (c) 2020-2022, Ink. All rights reserved.
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

#include "../../fLexer/fire-lexer.hpp"

#include <string>
#include <utility>
#include <vector>
#include <exception>
#include <iterator>

namespace ast {

class Node {
 public:
  virtual Node* GetChild(size_t) = 0;
  virtual size_t ChildrenSize() = 0;
  virtual std::vector<Node*>::iterator GetBeginIter() = 0;
  virtual std::string ToString(size_t) = 0;
};



class Leaf : public Node {
 protected:
  flexer::Token tok;

 public:

  Leaf() = default;

  explicit Leaf(const flexer::Token& tok) : tok{tok} {} // NOLINT

  flexer::Token GetToken();
  Node* GetChild(size_t i) override;
  size_t ChildrenSize() override;
  std::vector<Node*>::iterator GetBeginIter() override;
  std::string ToString(size_t depth) override;
};

class IntegerLiteral : public Leaf {
 public:
  using Leaf::Leaf;

  long long Value() noexcept;
};

class FloatLiteral : public Leaf {
 public:
  using Leaf::Leaf;

  long double Value() noexcept;
};

class CharLiteral : public Leaf {
 public:
  explicit CharLiteral(const flexer::Token& t);

  char Value();
};

class StringLiteral : public Leaf {
 public:
  using Leaf::Leaf;

  std::string Value();
};

class SubTree : public Node {
 protected:
  std::vector<Node*> children;

 public:
  SubTree() = default;

  explicit SubTree(std::vector<Node*> children) : children{children} {} // NOLINT

  Node* GetChild(size_t i) override;
  size_t ChildrenSize() override;
  std::vector<Node*>::iterator GetBeginIter() override;
  std::string ToString(size_t depth) override;
};

class BinaryExpr : public SubTree {
 public:
  using SubTree::SubTree;

  Node* LeftChild();
  flexer::Token Operator();
  Node* RightChild();
};

} // namespace ast

#endif // FIRELANG_AST_HPP
