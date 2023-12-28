#ifndef COMPILER_PARSINGTABLE_H
#define COMPILER_PARSINGTABLE_H

#include "../ContextFreeGrammarParser/CFGParser.h"
#include <iostream>


class ParsingTable {
public:
    Symbol getStartSymbol();
    Production getProduction(Symbol nonTerminal, Symbol terminal);
    bool isEmpty(Symbol nonTerminal, Symbol terminal);
    bool isSync(Symbol nonTerminal, Symbol terminal);
    void addProduction(Symbol nonTerminal, Symbol terminal, Production production);
    std::map<Symbol, std::map<Symbol, Production>> getParsingTable();
    void setStartSymbol(Symbol start);
private:
    std::map<Symbol, std::map<Symbol, Production>> parsingTable;
    Symbol start;

};

#endif //COMPILER_PARSINGTABLE_H
