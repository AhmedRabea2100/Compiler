#include "ParsingTable.h"
#include <algorithm>


Symbol ParsingTable::getStartSymbol() {
    return start;
}

void ParsingTable::setStartSymbol(Symbol start) {
    this->start = start;
}

Production ParsingTable::getProduction(Symbol nonTerminal, Symbol terminal) {
    return parsingTable[nonTerminal][terminal];
}

bool ParsingTable::isEmpty(Symbol nonTerminal, Symbol terminal) {
    auto it = parsingTable[nonTerminal].find(terminal);
    return (it == parsingTable[nonTerminal].end());
}

bool ParsingTable::isSync(Symbol nonTerminal, Symbol terminal) {
    Production prod = parsingTable[nonTerminal][terminal];
    return prod.productionSymbols[0].name == "sync";
}
void ParsingTable::addProduction(Symbol nonTerminal, Symbol terminal, Production production) {
    parsingTable[nonTerminal].insert(std::make_pair(terminal, production));
}


\
