#include "ParsingTableBuilder.h"
#include <queue>
#include <algorithm>

ParsingTableBuilder::ParsingTableBuilder()
{}

ParsingTable ParsingTableBuilder::buildParsingTable(map<Symbol, vector<Production>> grammar)
{
    // get first set of all non-terminal symbols
    map<Symbol, set<Symbol>> firstSet = getFirst(grammar);

    // get follow set of all non-terminal symbols
    map<Symbol, set<Symbol>> followSet = getFollow(grammar, firstSet);

    // construct parsing table
    constructParsingTable(grammar, firstSet, followSet);

    return parsingTable;
}

map<Symbol, set<Symbol>> ParsingTableBuilder::getFirst(map<Symbol, vector<Production>> grammar)
{
    map<Symbol, set<Symbol>> firstMap;
    map<Symbol, dependencies> dependenciesMap = getDependencies(grammar);

    // initialize first set of all terminal symbols to the symbol itself
    for (auto &rule : grammar)
    {
        for (auto &production : rule.second)
        {
            for (auto &symbol : production.productionSymbols)
            {
                if (symbol.type == TERMINAL || symbol.type == EPSILON)
                {
                    firstMap[symbol].insert(symbol);
                }
            }
        }
    }

    // initialize non-terminal queue with all non-terminal symbols that have no dependencies
    queue<Symbol> nonTerminalQueue;
    for (auto &rule : dependenciesMap)
    {
        if (rule.second.dependenciesCount == 0)
            nonTerminalQueue.push(rule.first);
    }

    // while the non-terminal queue is not empty
    while (!nonTerminalQueue.empty())
    {
        // get the first non-terminal symbol in the queue
        Symbol nonTerminal = nonTerminalQueue.front();
        nonTerminalQueue.pop();

        // for each production of the current non-terminal symbol
        for (auto &production : grammar[nonTerminal])
        {
            // if the production is epsilon
            if (production.productionSymbols.size() == 1 && production.productionSymbols[0].type == EPSILON)
            {
                firstMap[nonTerminal].insert(production.productionSymbols[0]);
            }
            else
            {
                for (auto &symbol : production.productionSymbols)
                {
                    if (symbol.type == TERMINAL)
                    {
                        firstMap[nonTerminal].insert(symbol);
                        break;
                    }
                    // if the symbol is a non-terminal symbol
                    else if (symbol.type == NON_TERMINAL || symbol.type == START)
                    {
                        // add the first set of the non-terminal symbol to the first set of the current non-terminal symbol
                        firstMap[nonTerminal].insert(firstMap[symbol].begin(), firstMap[symbol].end());

                        // break the loop if the non-terminal symbol doesn't have epsilon in its first set
                        if (firstMap[symbol].find(Symbol(EPSILON_SYMBOL, EPSILON)) == firstMap[symbol].end())
                        {
                            break;
                        }

                        // if the non-terminal symbol is the last symbol in the production
                        if (symbol == production.productionSymbols.back())
                        {
                            // add epsilon to the first set of the current non-terminal symbol
                            firstMap[nonTerminal].insert(Symbol(EPSILON_SYMBOL, EPSILON));
                        }
                    }
                }
            }
        }

        // for each non-terminal symbol that depends on the current non-terminal symbol
        for (auto &dependant : dependenciesMap[nonTerminal].dependants)
        {
            // decrement the number of dependencies of the non-terminal symbol
            dependenciesMap[dependant].dependenciesCount--;

            // if the non-terminal symbol has no dependencies
            if (dependenciesMap[dependant].dependenciesCount == 0)
            {
                // add the non-terminal symbol to the non-terminal queue
                nonTerminalQueue.push(dependant);
            }
        }
    }

    return firstMap;
}

map<Symbol, set<Symbol>> ParsingTableBuilder::getFollow(map<Symbol, vector<Production>> grammar, map<Symbol, set<Symbol>> firstSet)
{
    map<Symbol, set<Symbol>> followMap;

    bool update = true;
    size_t prevSize = 0;
    while (update)
    {
        update = false;
        for (auto &rule : grammar)
        {
            // if the current non-terminal symbol is the start symbol
            if (rule.first.type == START)
            {
                // add $ to the follow set of the start symbol
                prevSize = followMap[rule.first].size();
                followMap[rule.first].insert(Symbol(END_SYMBOL, TERMINAL));
                if (followMap[rule.first].size() > prevSize)
                    update = true;
            }
            for (auto &production : rule.second)
            {
                for (int i = 0; i < production.productionSymbols.size(); i++)
                {
                    // if the current symbol is a non-terminal symbol
                    if (production.productionSymbols[i].type == NON_TERMINAL || production.productionSymbols[i].type == START)
                    {
                        for (int j = i + 1; j < production.productionSymbols.size(); j++)
                        {
                            if (production.productionSymbols[j].type == TERMINAL)
                            {
                                // add the terminal symbol to the follow set of the current non-terminal symbol
                                prevSize = followMap[production.productionSymbols[i]].size();
                                followMap[production.productionSymbols[i]].insert(production.productionSymbols[j]);
                                if (followMap[production.productionSymbols[i]].size() > prevSize)
                                    update = true;
                                break;
                            }
                            // if the next symbol is a non-terminal symbol
                            else if (production.productionSymbols[j].type == NON_TERMINAL || production.productionSymbols[j].type == START)
                            {
                                // add the first set of the next symbol to the follow set of the current non-terminal symbol
                                prevSize = followMap[production.productionSymbols[i]].size();
                                followMap[production.productionSymbols[i]].insert(firstSet[production.productionSymbols[j]].begin(), firstSet[production.productionSymbols[j]].end());
                                if (followMap[production.productionSymbols[i]].size() > prevSize)
                                    update = true;

                                // break the loop if the next symbol doesn't have epsilon in its first set
                                if (firstSet[production.productionSymbols[j]].find(Symbol(EPSILON_SYMBOL, EPSILON)) == firstSet[production.productionSymbols[j]].end())
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
                
                for (int i = production.productionSymbols.size() - 1; i >= 0; i--)
                {
                    // if the current symbol is a non-terminal symbol
                    if (production.productionSymbols[i].type == NON_TERMINAL || production.productionSymbols[i].type == START)
                    {
                        // add the follow set of the current non-terminal symbol to the follow set of the symbol
                        prevSize = followMap[production.productionSymbols[i]].size();
                        followMap[production.productionSymbols[i]].insert(followMap[rule.first].begin(), followMap[rule.first].end());
                        if (followMap[production.productionSymbols[i]].size() > prevSize)
                            update = true;

                        // break the loop if the current non-terminal symbol doesn't have epsilon in its first set
                        if (firstSet[production.productionSymbols[i]].find(Symbol(EPSILON_SYMBOL, EPSILON)) == firstSet[production.productionSymbols[i]].end())
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    // remove epsilon from the follow set of all non-terminal symbols
    for (auto &rule : grammar)
    {
        followMap[rule.first].erase(Symbol(EPSILON_SYMBOL, EPSILON));
    }

    return followMap;
}

map<Symbol, ParsingTableBuilder::dependencies> ParsingTableBuilder::getDependencies(map<Symbol, vector<Production>> grammar)
{
    map<Symbol, dependencies> dependenciesMap;

    // initialize dependencies of all non-terminal symbols to empty set
    for (auto &rule : grammar)
    {
        dependenciesMap[rule.first].dependants = set<Symbol>();
        dependenciesMap[rule.first].dependenciesCount = 0;
    }

    for (auto &rule : grammar)
    {
        for (auto &production : rule.second)
        {
            for (auto &symbol : production.productionSymbols)
            {
                if (symbol.type == TERMINAL)
                    break;
                
                // if the current symbol is a non-terminal symbol
                if (symbol.type == NON_TERMINAL || symbol.type == START)
                {
                    // add the current non-terminal symbol to the dependants of the non-terminal symbol of the rule
                    dependenciesMap[symbol].dependants.insert(rule.first);

                    // increment the number of dependencies of the current non-terminal symbol
                    dependenciesMap[rule.first].dependenciesCount++;

                    // continue the loop if the current symbol has epsilon production
                    for (auto &prod : grammar[symbol])
                    {
                        if (production.productionSymbols.size() == 1 && production.productionSymbols[0].type == EPSILON)
                            continue;
                    }
                    break;
                }
            }
        }
    }

    return dependenciesMap;
}


void ParsingTableBuilder::constructParsingTable(std::map<Symbol, std::vector<Production>> rules, std::map<Symbol, std::set<Symbol>> first, std::map<Symbol, std::set<Symbol>> follow) {
    for (auto it = rules.begin(); it != rules.end(); it++) {
        // if the current non-terminal symbol is the start symbol
        if (it->first.type == START) {
            // set the start symbol of the parsing table
            parsingTable.setStartSymbol(it->first);
        }

        std::vector<Production> productions = rules[it->first];
        for (auto prod : productions) {
            std::vector<Symbol> symbols = prod.productionSymbols;
            // Check if the first symbol in the production can derive epsilon
            if (hasEpsilon(first[symbols[0]])) {
                // If epsilon is in the first set, add entries for each symbol in follow set
                for (auto terminal : follow[it->first]) {
                    // Check if the table entry is already occupied
                    if (!parsingTable.isEmpty(it->first, terminal)) {
                        std::cout << "Grammar is not LL1";
                        exit(0);
                    }
                    parsingTable.addProduction(it->first, terminal, prod);
                }
            } else {
                // If epsilon is not in the first set, add entries for each symbol in first set
                for (auto terminal : first[symbols[0]]) {
                    // Check if the table entry is already occupied
                    if (!parsingTable.isEmpty(it->first, terminal)) {
                        std::cout << "Grammar is not LL1";
                        exit(0);
                    }
                    parsingTable.addProduction(it->first, terminal, prod);
                }
            }
        }
    }
    // Add synchronization entries for each non-terminal symbol in follow set
    addSyncEntries(follow);
}

void ParsingTableBuilder::addSyncEntries(std::map<Symbol, std::set<Symbol>> follow) {
    Production sync;
    sync.productionSymbols.push_back(Symbol("SYNC", TERMINAL));
    for (auto f : follow) {
        for (auto s : f.second) {
            // Check if the table entry for the current non-terminal and terminal combination is empty
            if (parsingTable.isEmpty(f.first, s))
                // If it's empty, add the synchronization production to the parsing table
                parsingTable.addProduction(f.first, s, sync);
            else if (parsingTable.getProduction(f.first,s).productionSymbols[0].type == EPSILON)
                // Continue to the next terminal symbol without making any changes
                continue;
            else
                // If the table entry is not empty and doesn't start with EPSILON
                parsingTable.addProduction(f.first, s, sync);
        }
    }
}

bool ParsingTableBuilder::hasEpsilon(std::set<Symbol> symbols)
{
    return std::find(symbols.begin(), symbols.end(), Symbol(EPSILON_SYMBOL, EPSILON)) != symbols.end();
}
