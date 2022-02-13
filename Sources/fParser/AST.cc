// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#include "AST.hpp"

namespace ast {

void Node::Print(int depth) const {
  global::Log(std::cout, std::string(depth - 2, ' ') + "{");
  global::Log(std::cout, std::string(depth, ' ') + R"("node_type": )",
              0, false);

  nodetype == _leaf ? global::Log(std::cout, R"("leaf")")
                    : global::Log(std::cout, R"("node")");

  global::Log(std::cout, std::string(depth, ' ') + R"("token": )" + tok.ToString());

  if (nodetype == _node) {
    global::Log(std::cout, std::string(depth, ' ') + R"("sub_nodes": [)");
    for (const auto& i : this->sub_nodes) {
      i.Print(depth + 2);
    }
    global::Log(std::cout, std::string(depth, ' ') + "]");
  }

  global::Log(std::cout, std::string(depth - 2, ' ') + "}");
}



} // namespace ast