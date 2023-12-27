#pragma once

#include "../ContextFreeGrammarParser/CFGParser.h"
#include "ParsingTable.h"
#include <vector>
#include <map>
#include <set>
using namespace std;

class ParsingTableBuilder
{
public:
    ParsingTableBuilder();
    ParsingTable buildParsingTable(map<Symbol, vector<Production>> grammar);

private:
    struct dependencies
    {
        // depenants of a non-terminal symbol (A) are the non-terminal symbols that A appears in their first set
        set<Symbol> dependants;

        // number of non-terminal symbols that a non-terminal symbol (A) depends on
        int dependenciesCount;
    };
    
    ParsingTable parsingTable;
    map<Symbol, set<Symbol>> getFirst(map<Symbol, vector<Production>> grammar);
    map<Symbol, set<Symbol>> getFollow(map<Symbol, vector<Production>> grammar, map<Symbol, set<Symbol>> firstSet);
    map<Symbol, dependencies> getDependencies(map<Symbol, vector<Production>> grammar);
    void constructParsingTable(std::map<Symbol, std::vector<Production>> rules, std::map<Symbol, std::set<Symbol>> first, std::map<Symbol, std::set<Symbol>> follow);    
    void addSyncEntries(std::map<Symbol, std::set<Symbol>> follow);
    bool hasEpsilon(std::set<Symbol> symbols);
};
