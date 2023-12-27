#include "ParsingTable.h"
#include <algorithm>


Symbol ParsingTable::getStartSymbol() {
    return start;
}

void ParsingTable::setStartSymbol(Symbol start) {
    this->start = start;
}

Production ParsingTable::getProduction(Symbol nonTerminal, Symbol terminal) {
    return parsingTable[std::make_pair(nonTerminal, terminal)];
}

bool ParsingTable::isEmpty(Symbol nonTerminal, Symbol terminal) {
    return parsingTable.find(std::make_pair(nonTerminal, terminal)) == parsingTable.end();
}

void ParsingTable::addProduction(Symbol nonTerminal, Symbol terminal, Production production) {
    parsingTable[std::make_pair(nonTerminal, terminal)] = production;
}

bool ParsingTable::isSync(Symbol nonTerminal, Symbol terminal) {
    return parsingTable[std::make_pair(nonTerminal, terminal)].productionSymbols[0].name == "sync";
}

//void ParsingTable::addSync(std::map<Symbol, std::set<Symbol>> follow) {
//    for (auto &nonTerminal : follow) {
//        for (auto &terminal : f.second) {
//            // Create a pair representing the current non-terminal and the terminal symbol
//            std::pair<Symbol, Symbol> tableEntry(f.first, s);
//
//            // Check if the table entry is already occupied
//            if (parsingTable.find(tableEntry) != parsingTable.end()) {
//                // If the existing production is not epsilon, raise an error
//                if (parsingTable[tableEntry].productionSymbols[0].type != EPSILON) {
//                    std::cout << "Grammar is not LL(1)" << std::endl;
//                    exit(0);
//                }
//                // If the existing production is epsilon, continue to the next symbol
//                continue;
//            }
//
//            // Insert the synchronization entry directly into the parsing table
//            parsingTable[tableEntry].productionSymbols.push_back(Symbol("sync", TERMINAL));
//        }
//    }
//}
/*test*/
