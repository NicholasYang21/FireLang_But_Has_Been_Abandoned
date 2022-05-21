
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

#include "AST.hpp"

namespace ast {

std::string Types[2] = { // NOLINT
    "Null",
    "Primary"
};

inline bool AST::IsLeaf() const {
  return this->children.empty();
}

std::string AST::ToString() const {
  return this->ToJSON().dump(2);
}

nlohmann::json AST::ToJSON() const {
  ::nlohmann::json temp_json;

  temp_json["type"] = Types[static_cast<int>(this->type)];
  temp_json["curr_node"] = this->curr_node.ToJSON();

  if (this->children.empty())
    temp_json["children"] = {};
  else {
    for (const AST& i : this->children) {
      temp_json["children"].push_back(i.ToJSON());
    }
  }

  return temp_json;
}

} // namespace ast