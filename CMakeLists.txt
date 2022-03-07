cmake_minimum_required(VERSION 3.21)
project(MiniRDBMS)

set(CMAKE_CXX_STANDARD 20)

add_executable(MiniRDBMS main.cpp SQLInterpreter.cpp SQLInterpreter.h Scanner.cpp Scanner.h TokenType.cpp Token.cpp Token.h Parser.cpp Parser.h)
