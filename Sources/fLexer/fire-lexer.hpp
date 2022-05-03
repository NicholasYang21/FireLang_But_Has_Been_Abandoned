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

#ifndef FIRESCRIPT_FLEXER_HPP
#define FIRESCRIPT_FLEXER_HPP

#include <string>
#include <set>
#include <fstream>

namespace flexer {

enum Property {
  // Basic types
  // [Id], 123, 12.3, 'a', "abc", true/false
  Id, Int, Float, Char, String, Boolean,

  // Arithmetic Operators

    // Decimal
    // +, -, *, /, %
  Add, Sub, Mul, Div, Mod,

    // Binary (Bit Operation)
    // ~, &, |, ^, <<, >>, >>> (Right Arithmetic Shift)
  Not, And, Or, Xor, Lsh, Rsh, RAsh,

    // Self Operation
    // ++, --
  Incre, Decre,
    // +=, -=, *=, /=, %=
  AddAsg, SubAsg, MulAsg, DivAsg, ModAsg,
    // &=, |=, ^=, <<=, >>=, >>>=
  AndAsg, OrAsg, XorAsg, LshAsg, RshAsg, RAshAsg,

  // Logic Operators
  // !, ==, !=, &&, ||, <=, >=, <, >
  LNot, Eq, Ne, LAnd, LOr, Le, Ge, Les, Gr,

  // Access Operators
  // .
  Dot,

  // Type cast
  // ->
  Arrow,

  // Assign
  // =
  Assign,

  // Other Operators
  // ( ) [ ] { }
  LParen, RParen, LBracket, RBracket, LBrace, RBrace,
  // : ; ,
  Colon, Semicolon, Comma,

  // Identifiers
  EOF_, Unknown, Keyword, Comment
};

extern std::string Properties[58];
extern std::set<std::string> Keywords;

struct Token {
  Property property;
  std::string text, filename;

  unsigned line, col;

  [[nodiscard]] std::string ToString() const;
};

class Lexer {
 public:

  explicit Lexer(const std::string& filename);

  char Read();
  void Back(char);
  Token Automata();

  std::ifstream in_stm;
  std::string filename;

 private:
  Token Start();
  Token OctHex();
  Token Oct();
  Token Hex();
  Token Number();
  Token Identifier();
  Token Char_();
  Token String_();
  Token Comment_();
  Token LineComment();
  Token BlockComment();
  Token Symbol();

  unsigned line, col, last_col;
};

} // namespace flexer

#endif // FIRESCRIPT_FLEXER_HPP
