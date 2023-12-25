//
// Created by User on 12/25/2023.
//

#ifndef COMPILER_PARSINGTABLE_H
#define COMPILER_PARSINGTABLE_H

#include "CFGParser.h"

#include <iostream>


class ParsingTable {
    public:
        std::map<std::pair<Symbol, Symbol>, Production> parsingTable;
        Symbol getStartSymbol();
        Production getProduction(Symbol nonTerminal, Symbol terminal);
        bool isEmpty(Symbol nonTerminal, Symbol terminal);
        bool isSync(Symbol nonTerminal, Symbol terminal);

    private:

    void addProduction(Symbol nonTerminal, Symbol terminal, Production production);
    void setStartSymbol(Symbol start);
    Symbol start;
    void constructParsingTable(std::map<Symbol, std::vector<Production>> rules);
    bool hasEpsilon(std::vector<Symbol> symbols);

    void addSync(std::map<Symbol, std::set<Symbol>> follow);
};


#endif //COMPILER_PARSINGTABLE_H
