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

#include "fire-lexer.hpp"

#include "../shared/fire-error.hpp"
#include "../shared/global-functions.hpp"

namespace flexer {

std::string Properties[58] { // NOLINT
    // Basic types
    // [Id], 123, 12.3, 'a', "abc", true/false
    "Id", "Int", "Float", "Char", "String", "Boolean",

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
    // .
    "Dot",

    // Type cast
    // ->
    "Arrow",

    // Assign
    // =
    "Assign",

    // Other Operators
    // ( ) [ ] { }
    "LParen", "RParen", "LBracket", "RBracket", "LBrace", "RBrace",
    // : ; ,
    "Colon", "Semicolon", "Comma",

    // Identifiers
    "EOF_", "Unknown", "Keyword", "Comment"
};

std::set<std::string> Keywords { // NOLINT
  "integer",
  "char",
  "string",
  "float",
  "boolean",
  "none",

  "null",

  "final",

  "if",
  "then",
  "else",

  "switch",
  "case",
  "default",

  "for",
  "while",
  "repeat",
  "until",

  "fn",
  "return",

  "import",
  "use",
  "scope"
};

std::string Token::ToString() const {
  return R"({ "type": ")" + Properties[this->property] + R"(", "value": ")" +
        this->text + R"(", "line": )" +
         std::to_string(this->line) + R"(, "col": )" + std::to_string(this->col) + " }";
}

Lexer::Lexer(const std::string& filename) {
  in_stm = std::ifstream(filename, std::ios::in);

  this->filename = filename;

  this->line = this->col = 1, this->last_col = 0;
}

char Lexer::Read() {
  if (!in_stm.is_open()) {
    global::Log(std::cout, "Error: ", 2, false, true);
    global::Log(std::cout, "No such file or directory: ", 2,
                false, true);
    global::Log(std::cout, filename, 2, true, true);
    exit(0);
  }

  char ret = static_cast<char>(in_stm.get());

  if (ret == '\n')
    line++, last_col = col, col = 1;
  else if (ret == '\r')
    col = 1;
  else col++;

  return ret;
}

void Lexer::Back(char c) {
  in_stm.putback(c);
  col--;
  if (col == 0) col = last_col, line--;
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


Token Lexer::Automata() {
  return Start();
}

Token Lexer::Start() {
  char ch = Read();

  if (ch == '\0' || in_stm.eof()) {
    return {EOF_, "\0", filename, line, col}; // NOLINT
  } else if (ch == '0')
    return OctHex();
  else if (ch >= '1' && ch <= '9') {
    Back(ch);
    return Number();
  } else if (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
    Back(ch);
    return Identifier();
  } else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
    return Start();
  else if (ch == '\'')
    return Char_();
  else if (ch == '"')
    return String_();
  else if (ch == '/')
    return Comment_();
  else {
    Back(ch);
    return Symbol();
  }
}

Token Lexer::OctHex() {
  char ch = Read();

  if (ch >= '0' && ch <= '7') {
    Back(ch);
    return Oct();
  }
  else if (ch == 'x' || ch == 'X')
    return Hex();
  else
    return {Int, "0", filename, line, col};
}

Token Lexer::Oct() {
  char ch; std::string num = "0";

  while (ch = Read(), ch >= '0' && ch <= '7' && (!in_stm.eof())) {
    num += ch;
  }

  Back(ch);

  return {Int, num, filename, line, col};
}

Token Lexer::Hex() {
  char ch; std::string num = "0x";

  while (ch = Read(), (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
    && (!in_stm.eof())) {
    num += ch;
  }

  Back(ch);

  return {Int, num, filename, line, col};
}

Token Lexer::Number() {
  char ch; std::string num;
  int dot = 0;

  while (ch = Read(), (isdigit(ch) || (ch == '.' && dot == 0)) && (!in_stm.eof())) {
    if (ch == '.')
      dot++;

    num += ch;
  }

  Back(ch);

  if (dot == 1) return {Float, num, filename, line, col};
  return {Int, num, filename, line, col};
}

Token Lexer::Identifier() {
  char ch; std::string id;

  while (ch = Read(), (ch == '_' || isalpha(ch)) && (!in_stm.eof())) {
    id += ch;
  }

  if (Keywords.find(id) != Keywords.end()) return {Keyword, id, filename, line, col};
  else if (id == "true" || id == "false") return {Boolean, id, filename, line, col};

  Back(ch);
  return {Id, id, filename, line, col};
}

Token Lexer::Char_() {
  char ch; std::string char_;

  while (ch = Read(), ch != '\'' && (!in_stm.eof())) {
    char_ += ch;
  }

  return {Char, char_, filename, line, col};
}

Token Lexer::String_() {
  char ch; std::string str;

  while (ch = Read(), ch != '\"' && (!in_stm.eof())) {
    str += ch;
  }

  return {String, str, filename, line, col};
}

Token Lexer::Comment_() {
  char ch = Read();

  if (ch == '/') return LineComment();
  if (ch == '*') return BlockComment();

  Back(ch);
  return Symbol();
}

Token Lexer::LineComment() {
  while (Read() != '\n' && (!in_stm.eof()));
  return {Comment, "Comment", filename, line, col};
}

Token Lexer::BlockComment() {
  unsigned first_line = line, first_col = col - 2;
  char ch = Read(), last;

  while (last = ch, ch = Read(), (ch != '/' && last != '*') || (!in_stm.eof())) {
    if (ch == '/' && last == '*') break;

    if (in_stm.eof()) {
      std::string s =
        "unexpected EOF:\n  No '*/' to match '/*'.\nAt " + filename +
        " Line " + std::to_string(first_line) + " Col " + std::to_string(first_col)
      ;

      char* c = new char[strlen(s.c_str()) + 1];
      strcpy(c, s.c_str());

      throw global::FireError(c);
    }
  }

  return {Comment, "Comment", filename, line, col};
}

Token Lexer::Symbol() {
  char ch = Read();

  switch (ch) {
    case '+': {
      char t = Read();

      if (t == '=')
        return {AddAsg, "+=", filename, line, col};
      if (t == '+')
        return {Incre, "++", filename, line, col};

      Back(t);
      return {Add, "+", filename, line, col};
    }

    case '-': {
      char t = Read();

      if (t == '=')
        return {SubAsg, "-=", filename, line, col};
      if (t == '-')
        return {Decre, "--", filename, line, col};
      if (t == '>')
        return {Arrow, "->", filename, line, col};

      Back(t);
      return {Sub, "-", filename, line, col};
    }

    case '*': {
      char t = Read();

      if (t == '=')
        return {MulAsg, "*=", filename, line, col};

      Back(t);
      return {Mul, "*", filename, line, col};
    }

    case '/': {
      char t = Read();

      if (t == '=')
        return {DivAsg, "/=", filename, line, col};

      Back(t);
      return {Div, "/", filename, line, col};
    }

    case '%': {
      char t = Read();

      if (t == '=')
        return {ModAsg, "%=", filename, line, col};

      Back(t);
      return {Mod, "%", filename, line, col};
    }

    case '~':
      return {Not, "~", filename, line, col};

    case '&': {
      char t = Read();

      if (t == '=')
        return {AndAsg, "&=", filename, line, col};
      if (t == '&')
        return {LAnd, "&&", filename, line, col};

      Back(t);
      return {And, "&", filename, line, col};
    }

    case '|': {
      char t = Read();

      if (t == '=')
        return {OrAsg, "|=", filename, line, col};
      if (t == '|')
        return {LOr, "||", filename, line, col};

      Back(t);
      return {Or, "|", filename, line, col};
    }

    case '^': {
      char t = Read();

      if (t == '=')
        return {XorAsg, "^=", filename, line, col};

      Back(t);
      return {Xor, "^", filename, line, col};
    }

    case '<': {
      char t = Read();

      if (t == '<') {
        t = Read();

        if (t == '=')
          return {LshAsg, "<<=", filename, line, col};

        Back(t);
        return {Lsh, "<<", filename, line, col};
      }

      if (t == '=')
        return {Le, "<=", filename, line, col};

      Back(t);
      return {Les, "<", filename, line, col};
    }

    case '>': {
      char t = Read();

      if (t == '>') {
        t = Read();

        if (t == '=')
          return {RshAsg, ">>=", filename, line, col};

        if (t == '>') {
          t = Read();

          if (t == '=')
            return {RAshAsg, ">>>=", filename, line, col};

          Back(t);
          return {RAsh, ">>>", filename, line, col};
        }

        Back(t);
        return {Rsh, ">>", filename, line, col};
      }

      if (t == '=')
        return {Ge, ">=", filename, line, col};

      Back(t);
      return {Gr, ">", filename, line, col};
    }

    case '!': {
      char t = Read();

      if (t == '=')
        return {Ne, "!=", filename, line, col};

      Back(t);
      return {LNot, "!", filename, line, col};
    }

    case '=': {
      char t = Read();

      if (t == '=')
        return {Eq, "==", filename, line, col};

      Back(t);
      return {Assign, "=", filename, line, col};
    }

    case '.':
      return {Dot, ".", filename, line, col};

    case '(':
      return {LParen, "(", filename, line, col};

    case ')':
      return {RParen, ")", filename, line, col};

    case '[':
      return {LBracket, "[", filename, line, col};

    case ']':
      return {RBracket, "]", filename, line, col};

    case '{':
      return {LBrace, "{", filename, line, col};

    case '}':
      return {RBrace, "}", filename, line, col};

    case ':':
      return {Colon, ":", filename, line, col};

    case ';':
      return {Semicolon, ";", filename, line, col};

    case ',':
      return {Comma, ",", filename, line, col};

    default:
      break;
  }

  return {Unknown, std::string() + ch, filename, line, col};
}

} // namespace flexer