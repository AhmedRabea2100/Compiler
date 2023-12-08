#include <iostream>
#include <vector>
#include "./GrammarParser/GrammarParser.h"
#include "./GrammarParser/Token.h"
#include "./NFA/NFABuilder.h"
#include "./DFA/DFA.h"
#include "./CodeParser/CodeParser.h"
#include "./MinimizeDFA/Minimize.h"
#include "./TransitionTableWritter/TransitionTableWritter.h"

int main()
{
    // Read and parse grammar file
    GrammarParser grammarParser;
    std::vector<Token *> tokens = grammarParser.getTokens(R"(lexical_grammar.txt)");

    // Build NFA
    NFABuilder builder;
    NFA nfa = builder.build(tokens);
    unordered_set<char> inputs = builder.get_inputs();
    cout << "Finished building NFA" << endl;

    // Build DFA
    inputs.erase(EPSILON);
    DFA dfa;
    set<Dstate *> dfa_states = dfa.NFA_to_DFA({&nfa.start}, inputs);
    cout << "Finished building DFA" << endl;

    // Minimize DFA
    set<Dstate *> minimized_dfa = Minimize::minimize(dfa_states);
    cout << "Finished minimizing DFA" << endl;

    // Write transition table
    TransitionTableWritter writter;
    writter.write(minimized_dfa, inputs, "transition_table.txt");
    cout << "Finished writing transition table" << endl;
    
    // Parse code
    CodeParser codeParser(minimized_dfa);
    codeParser.parseFile("test_code.txt");
}
