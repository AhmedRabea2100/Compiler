#pragma once

#include "../ContextFreeGrammarParser/CFGParser.h"
#include <vector>
#include <map>
#include <set>
using namespace std;

class ParsingTableBuilder
{
public:
    ParsingTableBuilder();

private:
    struct dependencies
    {
        // depenants of a non-terminal symbol (A) are the non-terminal symbols that A appears in their first set
        set<Symbol> dependants;

        // number of non-terminal symbols that a non-terminal symbol (A) depends on
        int dependenciesCount;
    };
    
    map<Symbol, set<Symbol>> getFirst(map<Symbol, vector<Production>> grammar);
    map<Symbol, set<Symbol>> getFollow(map<Symbol, vector<Production>> grammar, map<Symbol, set<Symbol>> firstSet);
    map<Symbol, dependencies> getDependencies(map<Symbol, vector<Production>> grammar);
};
