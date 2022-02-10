// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#include "Symbol.hpp"
#include "../fLib/global-functions.hpp"

NormalEnv::NormalEnv(NormalEnv *prev) {
  this->prev = prev;
  this->table.clear();
}

void NormalEnv::AddSymbol(const std::string& id, const std::string& type,
                          const std::string& value) {
  if (type == "__UNKNOWN__") {
    global::Log(std::cout, "Error: An unknown type-id.", 2);
    exit(-1);
  } else {
    if (this->table.find(id) != this->table.end()) {
      global::Log(std::cout,
                  "Error: multiple definition of variable '" + id + "'.",
                  2);
      exit(0);
    }
    this->table.insert({id, {type, value}});
    return;
  }
}