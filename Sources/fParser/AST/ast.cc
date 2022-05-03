// This file is a part of FireLang.
// Copyright (c) 2021-2022, Ink. All rights reserved.
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

#include "ast.hpp"

#include "../../shared/global-functions.hpp"
#include "../../shared/fire-error.hpp"

namespace ast {

Node* Leaf::GetChild(size_t i) {
  throw global::FireError("Leaf node has no children.");
}

size_t Leaf::ChildrenSize() {
  return 0ULL;
}

std::vector<Node*>::iterator Leaf::GetBeginIter() {
  throw global::FireError("Leaf node has no children.");
}

std::string Leaf::ToString(size_t depth) {
  std::string indent(depth, ' '), inner_indent(depth + 2, ' ');
  return indent + "{\n" + inner_indent +
         "\"tok\": " + this->tok.ToString() + "\n" + indent + "}";
}

flexer::Token Leaf::GetToken() {
  return this->tok;
}

long long IntegerLiteral::Value() noexcept {
  return std::stoll(this->tok.text);
}

long double FloatLiteral::Value() noexcept {
  return std::stold(this->tok.text);
}

CharLiteral::CharLiteral(flexer::Token t) {
  if (t.text.size() > 1) {
    using st = std::string::size_type;
    for (st i = 0; i < t.text.size(); ++i) {
      if (t.text[i] == '\\')
        switch (t.text[++i]) {
          case 'n': {
            t.text.replace(i - 1, 2, "\n");
            break;
          }

          case 'r': {
            t.text.replace(i - 1, 2, "\r");
            break;
          }

          case 'b': {
            t.text.replace(i - 1, 2, "\b");
            break;
          }

          case 't': {
            t.text.replace(i - 1, 2, "\t");
            break;
          }

          case 'f': {
            t.text.replace(i - 1, 2, "\f");
            break;
          }

          case 'v': {
            t.text.replace(i - 1, 2, "\v");
            break;
          }

          case 'a': {
            t.text.replace(i - 1, 2, "\a");
            break;
          }

          case '0': {
            t.text.replace(i - 1, 2, "\0");
            break;
          }

          case '\\': {
            t.text.replace(i - 1, 2, "\\");
            break;
          }

          case '\'': {
            t.text.replace(i - 1, 2, "\'");
            break;
          }

          case '"': {
            t.text.replace(i - 1, 2, "\"");
            break;
          }

          case '?': {
            t.text.replace(i - 1, 2, "\?");
            break;
          }
        }
    }

    if (t.text.size() > 1) {
      std::string err = "Found an illegal character: \n  At " + t.filename +
                        " Line " + std::to_string(t.line)
                        + " Col " + std::to_string(t.col);
      CharLiteral::~CharLiteral();
      throw global::FireError(err.c_str());
    }
  }

  this->tok = t;
}

char CharLiteral::Value() {
  return this->tok.text[0];
}

std::string StringLiteral::Value() {
  return this->tok.text;
}

Node* SubTree::GetChild(size_t i) {
  return this->children.at(i);
}

size_t SubTree::ChildrenSize() {
  return this->children.size();
}

std::vector<Node*>::iterator SubTree::GetBeginIter() {
  return this->children.begin();
}

std::string SubTree::ToString(size_t depth) {
  std::string indent(depth, ' '), inner_indent(depth + 2, ' ');
  std::string ret = indent + "{\n" + inner_indent + R"("children": [)";

  for (auto & i : children) {
    ret += (i->ToString(depth + 4) + "," + "\n");
  }

  ret += inner_indent + "]\n" + indent + "}";

  return ret;
}

inline Node* BinaryExpr::LeftChild() {
  return this->children.at(0);
}

inline flexer::Token BinaryExpr::Operator() {
  return dynamic_cast<Leaf*>(this->children.at(1))->GetToken();
}

inline Node* BinaryExpr::RightChild() {
  return this->children.at(2);
}

} // namespace ast