cmake_minimum_required(VERSION 3.21)
project(MiniRDBMS)

set(CMAKE_CXX_STANDARD 20)

add_executable(MiniRDBMS main.cpp src/SQLInterpreter.cpp src/SQLInterpreter.h src/Scanner.cpp src/Scanner.h src/TokenType.cpp src/Token.cpp src/Token.h src/Parser.cpp src/Parser.h src/Stmt.cpp)
