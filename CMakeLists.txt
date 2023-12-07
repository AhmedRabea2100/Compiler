cmake_minimum_required(VERSION 3.10)
project(Compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(Compiler main.cpp
        GrammarParser/Token.cpp
        GrammarParser/GrammarParser.cpp
        NFA/State.cpp
        NFA/Transition.cpp
        NFA/NFA.cpp
        NFA/NFABuilder.cpp
)
