cmake_minimum_required(VERSION 3.10)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(Compiler main.cpp
        DFA.h
        DFA.cpp
        Dstates.h
        Dstates.cpp Minimize.cpp Minimize.h

        GrammarParser/Token.cpp
        GrammarParser/GrammarParser.cpp
        state.cpp
        transition.cpp
        nfa.cpp
        nfa-builder.cpp
        CodeParser.cpp
)
