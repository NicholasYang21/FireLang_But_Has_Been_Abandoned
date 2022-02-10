// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#ifndef FIRESCRIPT_FLEXER_HPP
#define FIRESCRIPT_FLEXER_HPP

#include <string>
#include <set>
#include <iostream>

namespace fLexer {

enum Property {
  // Basic types
  // [Id], 123, 12.3, 'a', "abc"
  Id, Int, Float, Char, String,

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

extern std::string Properties[57];
extern std::set<std::string> Keywords;

struct Token {
  Property property;
  std::string text;

  unsigned line, col;

  [[nodiscard]] std::string ToString() const;
};

class Lexer {
 public:

  explicit Lexer(std::istream& stm) :
    in_stm {stm} { line = col = 1; last_col = 0; };

  char Read();
  void Back(char);
  Token Automata();

  std::istream& in_stm;

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

} // namespace fLexer

#endif // FIRESCRIPT_FLEXER_HPP
