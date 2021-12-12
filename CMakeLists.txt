cmake_minimum_required(VERSION 3.20)
project(FireScript)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall")

add_executable(Fire fMain.cpp Sources/fLib/global-functions.h Sources/fLib/global-functions.cc Sources/fLexer/fLexer.h Sources/fLexer/fLexer.cc)
