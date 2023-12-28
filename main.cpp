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


std::ofstream file;
std::string fileName = "yousry222.txt";

void fillSpaces(int spaceNum) {
    for (int i = 0; i < spaceNum; i++)
        file << " ";
}

void drawLine(int length) {
    for (int i = 0; i < length; i++)
        file << "-";
    file << "\n";
}

void writeParsingTable(ParsingTable parsingTable) {
    std::map<Symbol, std::map<Symbol, Production>> table;
    table = parsingTable.getParsingTable();
    file.open(fileName);
    file << "PREDICTIVE PARSING TABLE :" << std::endl;
    drawLine(table.size() * 50);
    //first print terminals
    std::set<Symbol> terminals;
    for (auto &outer_map_pair : table)
        for (auto &inner_map_pair : outer_map_pair.second)
            terminals.insert(inner_map_pair.first);
    fillSpaces(45);
    for (auto terminal : terminals) {
        if (terminal.name == "`")
            file << "$";
        else
            file << terminal.name;
        fillSpaces(50 - terminal.name.length());
    }
    file << std::endl;
    drawLine(table.size() * 50);


    for (auto &outer_map_pair : table) {
        file << outer_map_pair.first.name;
        fillSpaces(20 - outer_map_pair.first.name.length());
        file << "|";
        for (auto terminal : terminals) {
            if (!parsingTable.isEmpty(outer_map_pair.first, terminal)) {
                std::string production = table[outer_map_pair.first][terminal].stringify();
                file << production;
                fillSpaces(50 - production.length());
                file << "|";
            } else {
                fillSpaces(50);
                file << "|";
            }
        }
        file << std::endl;
    }

    drawLine(table.size() * 50);
    file << std::endl;
    file.close();
}



int main(){
    // Read and parse grammar file
    GrammarParser grammarParser;
    std::vector<Token *> tokens = grammarParser.getTokens(R"(lexical_grammar.txt)");

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
    writeParsingTable(parsingTable);

    // Parse code and Left Derivation
    auto *parserWritter = new OutputWritter("parser_output.txt");
    auto *leftDerivationWritter = new OutputWritter("left_derivation.txt");
    auto p = &parsingTable;
    Parser::getInstance().parse(p, tokenizer, parserWritter, leftDerivationWritter);
    std::cout << "Finished Matching & Parsing\n";

    lexicalAnalyzerWritter->closeFile();
    parserWritter->closeFile();
    leftDerivationWritter->closeFile();
}