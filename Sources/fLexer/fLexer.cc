// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#include "fLexer.h"

namespace fLexer {

std::string Properties[54] {
    // Basic types
    // [Id], 123, 12.3, 'a', "abc"
    "Id", "Int", "Float", "Char", "String",

    // Arithmetic Operators

    // 10-Based
    // +, -, *, /, %
    "Add", "Sub", "Mul", "Div", "Mod",

    // 2-Based (Bit Operation)
    // ~, &, |, ^, <<, >>, >>> (Right Arithmetic Shift)
    "Not", "And", "Or", "Xor", "Lsh", "Rsh", "RAsh",

    // Self Operation
    // ++, --
    "Incre", "Decre",
    // +=, -=, *=, /=, %=
    "AddAsg", "SubAsg", "MulAsg", "DivAsg", "ModAsg",
    // &=, |=, ^=, <<=, >>=, >>>=
    "AndAsg", "OrAsg", "XorAsg", "LshAsg", "RshAsg", "RAshAsg",

    // Logic Operators
    // !, ==, !=, &&, ||, <=, >=, <, >
    "LNot", "Eq", "Ne", "LAnd", "LOr", "Le", "Ge", "Les", "Gr",

    // Access Operators
    // ., ->
    "Dot", "Arrow",

    // Other Operators
    // ( ) [ ] { }
    "LParen", "RParen", "LBracket", "RBracket", "LBrace", "RBrace",
    // : ; ,
    "Colon", "Semicolon", "Comma",

    // Identifiers
    "EOF_", "Unknown", "Error", "Keyword"
};

std::string Token::ToString() {
  return "<" + Properties[this->propertie] + ", \'" + this->text + "\'>";
}

char Lexer::Read() {
  char ret = this->in_stm.get();

  if (ret == '\n')
    line++, col = 1;
  else if (ret == '\r')
    col = 1;
  else col++;

  return ret;
}

// Working process:
// Keys:
//  %a%  indicates when read a character is 'a'. (first character of a new word)
//  ->   indicates goto the δ.
//  a()  indicates the δ.
//  <a>  indicates the part of speech of the word (in Σ).
//  [a]  indicates the keyword.
//  Skip indicates skip current character.
//  Back indicates turn the character back to the input stream.
//
// Start():
//  %0% -> OctHex()
//  %[1-9]% -> Number()
//  %[a-z | A-Z | _]% -> Identifier()
//  %[\n | \t | ' ']% -> Skip
//  %/% -> Comment()
//  %[Other]% -> Symbol()
//  %\0% -> End
//
// OctHex():
//  %[0-7]% -> Oct()
//  %x% -> Hex()
//
// Oct():
//  %[0-7]% -> Oct()
//  %[Other]% -> <Num>
//
// Hex():
//  Skip, Skip
//  %[0-9 | a-f | A-F]% -> Hex()
//  %[Other]% -> <Num>
//
// Number():
//  %[0 - 9]% -> Number()
//  %[Other]% -> <Num>
//
// Identifier():
//  %[a-z | A-Z | _]% -> Identifier()
//  %[Other]% -> <Id>
//
// Comment():
//  %/% -> LineComment()
//  %*% -> BlockComment()


inline Token Lexer::Automata() {
  return Start();
}

Token Lexer::Start() {
  char ch = Read();

  if (ch == '0')
    return OctHex();
  else if (ch >= '1' && ch <= '9')
    return Number();
  else if (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    return Identifier();
  else if (ch == ' ' || ch == '\n' || ch == '\t')
    return Start();
  else if (ch == '\'')
    return Char();
  else if (ch == '"')
    return String();
  else if (ch == '/')
    return Comment();
  else if (ch == '\0')
    return {EOF_, "\0", line, col};

  return Symbol();
}

} // namespace fLexer