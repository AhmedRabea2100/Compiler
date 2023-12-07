#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include "./GrammarParser/GrammarParser.h"
#include "./GrammarParser/Token.h"
#include "./NFA/NFABuilder.h"
#include "./DFA/DFA.h"
#include "./CodeParser/CodeParser.h"
#include "Minimize.h"

int main() {
    GrammarParser grammarParser;
    // Read and parse grammar file
    std::vector<Token *> tokens = grammarParser.getTokens(
            R"(M:\CSED\YEAR 4\Compilers\Project\Compiler\lexical_grammar.txt)");
    for (auto &token: tokens) {
        std::cout << token->getType() << " " << token->getPriority() << std::endl;
        for (auto &regexChar: token->getPostfixRegex()) {
            std::cout << regexChar->c << "(" << regexChar->cType << ")" << " ";
        }
        std::cout << std::endl;
    }

    NFABuilder builder;
    NFA nfa = builder.build(tokens);
    unordered_set<char> inputs = builder.get_inputs();
    inputs.erase(' ');

    DFA dfa;

    auto start_time = std::chrono::high_resolution_clock::now();
    set<Dstate *> dfa_states = dfa.NFA_to_DFA({&nfa.start}, inputs);
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    int c=0;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    for(auto &state : dfa_states) {
        if(!state->get_is_final()) {
            cout << "DFA   " << state->get_id() << "  " << state->get_is_final() << endl;
            c++;
        }
    }
    cout << "DFA   " << c << endl;
    // Output the duration in microseconds
    std::cout << "Time taken: " << duration.count() << " microseconds" << std::endl;
    cout << "DFA   " << dfa_states.size() << endl;

    set<Dstates *> minimized_dfa = Minimize::minimize(dfa_states);
    cout << "Minimized DFA  " << minimized_dfa.size() << endl;

//    CodeParser codeParser(dfa_states);
//    codeParser.parseFile("D:/Last year/Project/Compiler/test_code.txt");
    cout << "oooooo" << endl;

}
