cmake_minimum_required(VERSION 3.20)

project(FireLang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Fire fire-main.cc Sources/fLexer/fire-lexer.hpp Sources/fLexer/fire-lexer.cc Sources/shared/global-functions.hpp Sources/shared/global-functions.cc Sources/shared/fire-error.hpp Sources/shared/fire-error.cc Sources/fParser/AST.cc Sources/fParser/AST.hpp JSON/json.hpp Sources/fParser/Parsers/primary-node.hpp Sources/fParser/Parsers/nodes-interface.hpp)
