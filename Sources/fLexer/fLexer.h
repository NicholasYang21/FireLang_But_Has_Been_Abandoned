// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#ifndef FIRESCRIPT_FLEXER_H
#define FIRESCRIPT_FLEXER_H

#include <string>
#include <iostream>

namespace fLexer {

enum Property {
  // Basic types
  // [Id], 123, 12.3, 'a', "abc"
  Id, Int, Float, Char, String,

  // Arithmetic Operators

    // 10-Based
    // +, -, *, /, %
  Add, Sub, Mul, Div, Mod,

    // 2-Based (Bit Operation)
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
  // ., ->
  Dot, Arrow,

  // Other Operators
  // ( ) [ ] { }
  LParen, RParen, LBracket, RBracket, LBrace, RBrace,
  // : ; ,
  Colon, Semicolon, Comma,

  // Identifiers
  EOF_, Unknown, Error, Keyword
};

extern std::string Properties[54];

struct Token {
  Property propertie;
  std::string text;

  unsigned line, col;

  std::string ToString();
};

class Lexer {
 public:

  explicit Lexer(std::istream& stm) :
    in_stm {stm} { line = col = 1; };

  char Read();
  Token Automata();

  std::istream& in_stm;

 private:
  Token Start();
  Token OctHex();
  Token Oct();
  Token Hex();
  Token Number();
  Token Identifier();
  Token Char();
  Token String();
  Token Comment();
  Token LineComment();
  Token BlockComment();
  Token Symbol();

  static unsigned line, col;
};

} // namespace fLexer

#endif // FIRESCRIPT_FLEXER_H
