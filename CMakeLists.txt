cmake_minimum_required(VERSION 3.20)

project(FireScript)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Fire fMain.cc Sources/fLib/global-functions.cc Sources/fLexer/fLexer.cc)
