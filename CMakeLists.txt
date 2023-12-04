cmake_minimum_required(VERSION 3.23)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(Compiler main.cpp
        GrammarParser/Token.cpp
        GrammarParser/Token.h
        GrammarParser/GrammarParser.cpp
        GrammarParser/GrammarParser.h
)
