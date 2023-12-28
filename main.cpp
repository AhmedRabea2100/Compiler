#include <iostream>
#include <vector>
#include "./GrammarParser/GrammarParser.h"
#include "./GrammarParser/Token.h"
#include "./NFA/NFABuilder.h"
#include "./DFA/DFA.h"
#include "./LexicalCodeParser/CodeParser.h"
#include "./MinimizeDFA/Minimize.h"
#include "./OutputWritter/OutputWritter.h"
#include "./LexicalCodeParser/Tokenizer.h"
#include "./ContextFreeGrammarParser/CFGParser.h"
#include "./LL1Parser/ParsingTableBuilder.h"
#include "./Parser/parser.h"


int main(){
    // Read and parse grammar file
    GrammarParser grammarParser;
    std::vector<Token *> tokens = grammarParser.getTokens("lexical_grammar.txt");

    // Build NFA
    NFABuilder builder;
    NFA nfa = builder.build(tokens);
    unordered_set<char> inputs = builder.get_inputs();
    cout << "Finished building NFA" << endl;

    // Build DFA
    inputs.erase(EPSILON_CHAR);
    DFA dfa;
    set<Dstate *> dfa_states = dfa.NFA_to_DFA({&nfa.start}, inputs);
    cout << "Finished building DFA" << endl;

    // Minimize DFA
    set<Dstate *> minimized_dfa = Minimize::minimize(dfa_states);
    cout << "Finished minimizing DFA" << endl;

    // Write transition table
    OutputWritter tableWritter("transition_table.txt");
    tableWritter.writeTransitionTable(minimized_dfa, inputs);
    tableWritter.closeFile();
    cout << "Finished writing transition table" << endl;

    // -----------------------------------------------------------------------------------------------------------------------------------

    // Parse code
    auto *lexicalAnalyzerWritter = new OutputWritter("lexical_analyzer_output.txt");
    auto *tokenizer = new Tokenizer(minimized_dfa, "test_code.txt", lexicalAnalyzerWritter);

    // Parse Context Free Grammar
    CFGParser cfgParser;
    map<Symbol, vector<Production>> grammar = cfgParser.getRules("context_free_grammar.txt");

    // Build Parsing Table
    ParsingTableBuilder parsingTableBuilder;
    ParsingTable parsingTable = parsingTableBuilder.buildParsingTable(grammar);
    cout << "Finished building parsing table" << endl;
    
    // Write Parsing Table
    auto *table = new OutputWritter("parsing_table.txt");
    table->writeParsingTable(&parsingTable);
    table->closeFile();
    cout << "Finished writing parsing table" << endl;

    // Parse code and Left Derivation
    auto *parserWritter = new OutputWritter("parser_output.txt");
    auto *leftDerivationWritter = new OutputWritter("left_derivation.txt");
    Parser::getInstance().parse(&parsingTable, tokenizer, parserWritter, leftDerivationWritter);
    std::cout << "Finished Matching & Parsing" << endl;

    lexicalAnalyzerWritter->closeFile();
    parserWritter->closeFile();
    leftDerivationWritter->closeFile();
}
