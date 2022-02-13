cmake_minimum_required(VERSION 3.20)

project(FireLang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Fire fMain.cc Sources/fLexer/fLexer.hpp Sources/fLexer/fLexer.cc Sources/fLib/global-functions.hpp Sources/fLib/global-functions.cc Sources/fParser/AST.hpp Sources/fParser/AST.cc Sources/fParser/fParser.hpp Sources/fParser/fParser.cc Sources/fParser/Symbol.hpp Sources/fParser/Symbol.cc)
