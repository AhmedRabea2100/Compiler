#ifndef COMPILER_PARSINGTABLE_H
#define COMPILER_PARSINGTABLE_H

#include "../ContextFreeGrammarParser/CFGParser.h"
#include <iostream>


class ParsingTable {
    public:
        std::map<std::pair<Symbol, Symbol>, Production> parsingTable;
        Symbol getStartSymbol();
        Production getProduction(Symbol nonTerminal, Symbol terminal);
        bool isEmpty(Symbol nonTerminal, Symbol terminal);
        bool isSync(Symbol nonTerminal, Symbol terminal);
        void addProduction(Symbol nonTerminal, Symbol terminal, Production production);

    private:
        Symbol start;
        void setStartSymbol(Symbol start);
};

#endif //COMPILER_PARSINGTABLE_H
