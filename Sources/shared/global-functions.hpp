// This file is a part of FireScript.
// Copyright (c) 2021, Ink. All rights reserved.

extern bool DEBUGGING;
extern const char* BUILD_VER;

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../fLexer/fire-lexer.hpp"
#include "../fParser/AST.hpp"

// color table
#if defined(_WIN32) || defined(_WIN64)

// winAPI
#include <windows.h>

enum COLORS {
  RED = FOREGROUND_RED,
  GREEN = FOREGROUND_GREEN,
  BLUE = FOREGROUND_BLUE,
  YELLOW = RED | GREEN,
  VIOLET = RED | BLUE,
  CYAN = GREEN | BLUE,
  WHITE = RED | GREEN | BLUE,

  BK_RED = BACKGROUND_RED,
  BK_GREEN = BACKGROUND_GREEN,
  BK_BLUE = BACKGROUND_BLUE,
  BK_YELLOW = BK_RED | BK_GREEN,
  BK_VIOLET = BK_RED | BK_BLUE,
  BK_CYAN = BK_GREEN | BK_BLUE,
  BK_WHITE = BK_RED | BK_GREEN | BK_BLUE,

  BLACK = 0x0,
  BK_BLACK = 0x0
};

#define SetColor(C) SetConsoleTextAttribute( \
  GetStdHandle(STD_OUTPUT_HANDLE), (DWORD)(C))

#else

#define RED "\033[38;2;255;0;0"
#define GREEN "\033[38;2;0;255;0"
#define BLUE "\033[38;2;0;0;255"
#define YELLOW "\033[38;2;255,255,0"
#define VIOLET "\033[38;2;255;0;255"
#define CYAN "\033[38;2;0;255;255"
#define WHITE "\033[38;2;255;255;255"
#define BLACK "\033[38;2;0;0;0"

#define BK_RED "\033[48;2;255;0;0"
#define BK_GREEN "\033[48;2;0;255;0"
#define BK_BLUE "\033[48;2;0;0;255"
#define BK_YELLOW "\033[48;2;255,255,0"
#define BK_VIOLET "\033[48;2;255;0;255"
#define BK_CYAN "\033[48;2;0;255;255"
#define BK_WHITE "\033[48;2;255;255;255"
#define BK_BLACK "\033[48;2;0;0;0"

#define SetColor(C) std::cout << C << std::endl

#endif // defined(WIN32) || defined(_WIN64)

namespace global {

// Level = 0: info
// Level = 1: Warning
// Level >= 2: Error
void Log(
         std::ostream& s, const std::string& content, int level = 0, bool endl = true,
         bool UI = false
        );

void ProcessParams(int argc, char** argv);

} // namespace global