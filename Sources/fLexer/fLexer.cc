// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

#include "fLexer.h"
#include "../fLib/global-functions.h"

namespace fLexer {

std::string Properties[57] {
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
    "EOF_", "Unknown", "Error", "Keyword", "Comment"
};

std::set<std::string> Keywords{
  "integer",
  "char",
  "string",
  "float",
  "none",

  "bool",
  "true",
  "false",
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

std::string Token::ToString() {
  return "<" + Properties[this->propertie] + ", \'" + this->text + "\' Line " +
         std::to_string(this->line) + " Col " + std::to_string(this->col) + ">";
}

char Lexer::Read() {
  char ret = this->in_stm.get();

  if (ret == '\n')
    line++, last_col = col, col = 1;
  else if (ret == '\r')
    col = 1;
  else col++;

  return ret;
}

void Lexer::Back(char c) {
  this->in_stm.putback(c);
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
    return {EOF_, "\0", line, col};
    exit(0);
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
    return {Int, "0", line, col};
}

Token Lexer::Oct() {
  char ch; std::string num = "0";

  while (ch = Read(), ch >= '0' && ch <= '7' && (!in_stm.eof())) {
    num += ch;
  }

  Back(ch);

  return {Int, num, line, col};
}

Token Lexer::Hex() {
  char ch; std::string num = "0x";

  while (ch = Read(), (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
    && (!in_stm.eof())) {
    num += ch;
  }

  Back(ch);

  return {Int, num, line, col};
}

Token Lexer::Number() {
  char ch; std::string num = "";
  int dot = 0;

  while (ch = Read(), (isdigit(ch) || (ch == '.' && dot == 0)) && (!in_stm.eof())) {
    if (ch == '.')
      dot++;

    num += ch;
  }

  Back(ch);

  if (dot == 1) return {Float, num, line, col};
  return {Int, num, line, col};
}

Token Lexer::Identifier() {
  char ch; std::string id = "";

  while (ch = Read(), (ch == '_' || isalpha(ch)) && (!in_stm.eof())) {
    id += ch;
  }

  if (Keywords.find(id) != Keywords.end()) return {Keyword, id, line, col};
  Back(ch);
  return {Id, id, line, col};
}

Token Lexer::Char_() {
  char ch; std::string char_ = "";

  while (ch = Read(), ch != '\'' && (!in_stm.eof())) {
    char_ += ch;
  }

  return {Char, char_, line, col};
}

Token Lexer::String_() {
  char ch; std::string str = "";

  while (ch = Read(), ch != '\"' && (!in_stm.eof())) {
    str += ch;
  }

  return {String, str, line, col};
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
  return {Comment, "Comment", line, col};
}

Token Lexer::BlockComment() {
  unsigned firstline = line, firstcol = col - 2;
  char ch = Read(), last;

  while (last = ch, ch = Read(), (ch != '/' && last != '*') || (!in_stm.eof())) {
    if (ch == '/' && last == '*') break;

    if (in_stm.eof()) {
      global::Log(std::cout, "unexcepted EOF:", 2, true);
      global::Log(std::cout, "  No '*/' to match '/*'", 2, true);
      global::Log(std::cout, "At Line " + std::to_string(firstline) + " Col " +
                  std::to_string(firstcol), 2, true);
      exit(-1);
    }
  }

  return {Comment, "Comment", line, col};
}

Token Lexer::Symbol() {
  char ch = Read();

  switch (ch) {
    case '+': {
      char t = Read();

      if (t == '=')
        return {AddAsg, "+=", line, col};
      if (t == '+')
        return {Incre, "++", line, col};

      Back(t);
      return {Add, "+", line, col};
    }

    case '-': {
      char t = Read();

      if (t == '=')
        return {SubAsg, "-=", line, col};
      if (t == '-')
        return {Decre, "--", line, col};
      if (t == '>')
        return {Arrow, "->", line, col};

      Back(t);
      return {Sub, "-", line, col};
    }

    case '*': {
      char t = Read();

      if (t == '=')
        return {MulAsg, "*=", line, col};

      Back(t);
      return {Mul, "*", line, col};
    }

    case '/': {
      char t = Read();

      if (t == '=')
        return {DivAsg, "/=", line, col};

      Back(t);
      return {Div, "/", line, col};
    }

    case '%': {
      char t = Read();

      if (t == '=')
        return {ModAsg, "%=", line, col};

      Back(t);
      return {Mod, "%", line, col};
    }

    case '~':
      return {Not, "~", line, col};

    case '&': {
      char t = Read();

      if (t == '=')
        return {AndAsg, "&=", line, col};
      if (t == '&')
        return {LAnd, "&&", line, col};

      Back(t);
      return {And, "&", line, col};
    }

    case '|': {
      char t = Read();

      if (t == '=')
        return {OrAsg, "|=", line, col};
      if (t == '|')
        return {LOr, "||", line, col};

      Back(t);
      return {Or, "|", line, col};
    }

    case '^': {
      char t = Read();

      if (t == '=')
        return {XorAsg, "^=", line, col};

      Back(t);
      return {Xor, "^", line, col};
    }

    case '<': {
      char t = Read();

      if (t == '<') {
        t = Read();

        if (t == '=')
          return {LshAsg, "<<=", line, col};

        Back(t);
        return {Lsh, "<<", line, col};
      }

      if (t == '=')
        return {Le, "<=", line, col};

      Back(t);
      return {Les, "<", line, col};
    }

    case '>': {
      char t = Read();

      if (t == '>') {
        t = Read();

        if (t == '=')
          return {RshAsg, ">>=", line, col};

        if (t == '>') {
          t = Read();

          if (t == '=')
            return {RAshAsg, ">>>=", line, col};

          Back(t);
          return {RAsh, ">>>", line, col};
        }

        Back(t);
        return {Rsh, ">>", line, col};
      }

      if (t == '=')
        return {Ge, ">=", line, col};

      Back(t);
      return {Gr, ">", line, col};
    }

    case '!': {
      char t = Read();

      if (t == '=')
        return {Ne, "!=", line, col};

      Back(t);
      return {LNot, "!", line, col};
    }

    case '=': {
      char t = Read();

      if (t == '=')
        return {Eq, "==", line, col};

      Back(t);
      return {Assign, "=", line, col};
    }

    case '.':
      return {Dot, ".", line, col};

    case '(':
      return {LParen, "(", line, col};

    case ')':
      return {RParen, ")", line, col};

    case '[':
      return {LBracket, "[", line, col};

    case ']':
      return {RBracket, "]", line, col};

    case '{':
      return {LBrace, "{", line, col};

    case '}':
      return {RBrace, "}", line, col};

    case ':':
      return {Colon, ":", line, col};

    case ';':
      return {Semicolon, ";", line, col};

    case ',':
      return {Comma, ",", line, col};

    default:
      break;
  }

  return {Unknown, std::string() + ch, line, col};
}

} // namespace fLexer