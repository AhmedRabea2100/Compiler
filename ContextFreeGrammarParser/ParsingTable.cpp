//
// Created by User on 12/25/2023.
//

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

void ParsingTable::constructParsingTable(std::map<Symbol, std::vector<Production>> rules) {
    for (auto &rule : rules) {
        std::vector<Production> productions = rule.second;
        for(auto &production : productions) {
            std::vector<Symbol> symbols = production.productionSymbols;
            if(hasEpsilon(first[symbols[0]])){
                // add each symbol in the Follow set
                for(auto &terminal : follow[rule.first]){
                    std::pair<Symbol, Symbol> tableEntry(rule.first, terminal);
                    if(parsingTable.find(tableEntry) != parsingTable.end()){
                        std::cout << "Grammar is not LL(1)" << std::endl;
                        exit(0);
                    }
                    addProduction(rule.first, terminal, production);
                }
            }else{
                // add each symbol in the First set
                for(auto &terminal : first[symbols[0]]){
                    std::pair<Symbol, Symbol> tableEntry(rule.first, terminal);
                    if(parsingTable.find(tableEntry) != parsingTable.end()){
                        std::cout << "Grammar is not LL(1)" << std::endl;
                        exit(0);
                    }
                    addProduction(rule.first, terminal, production);
                }
            }

        }


    }
    addSync(follow);
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
void ParsingTable::addSync(std::map<Symbol, std::set<Symbol>> follow) {
    for (auto &nonTerminal : follow) {
        for (auto &terminal : nonTerminal.second) {
            std::pair<Symbol, Symbol> tableEntry(nonTerminal.first, terminal);
            if (parsingTable.find(tableEntry) != parsingTable.end()) {
                if (parsingTable[tableEntry].productionSymbols[0].type != EPSILON) {
                    std::cout << "Grammar is not LL(1)" << std::endl;
                    exit(0);
                }
                continue;
            }
            parsingTable[tableEntry].productionSymbols.push_back(Symbol("sync", TERMINAL));
        }

    }
}



bool ParsingTable::hasEpsilon(std::vector<Symbol> symbols) {
    return std::find(symbols.begin(), symbols.end(), Symbol("EPSILON", EPSILON)) != symbols.end();
}





