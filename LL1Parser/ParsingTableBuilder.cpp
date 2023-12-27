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

    // initialize first of all non-terminal symbols to empty set
    for (auto &rule : grammar)
    {
        firstMap[rule.first] = set<Symbol>();
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
}

map<Symbol, set<Symbol>> ParsingTableBuilder::getFollow(map<Symbol, vector<Production>> grammar, map<Symbol, set<Symbol>> firstSet)
{
    map<Symbol, set<Symbol>> followMap;

    // initialize follow of all non-terminal symbols to empty set
    for (auto &rule : grammar)
    {
        followMap[rule.first] = set<Symbol>();
    }

    for (auto &rule : grammar)
    {
        // if the current non-terminal symbol is the start symbol
        if (rule.first.type == START)
        {
            // add $ to the follow set of the start symbol
            followMap[rule.first].insert(Symbol("$", TERMINAL));
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
                            followMap[production.productionSymbols[i]].insert(production.productionSymbols[j]);
                            break;
                        }
                        // if the next symbol is a non-terminal symbol
                        else if (production.productionSymbols[j].type == NON_TERMINAL || production.productionSymbols[j].type == START)
                        {
                            // add the first set of the next symbol to the follow set of the current non-terminal symbol
                            followMap[production.productionSymbols[i]].insert(firstSet[production.productionSymbols[j]].begin(), firstSet[production.productionSymbols[j]].end());

                            // break the loop if the next symbol doesn't have epsilon in its first set
                            if (firstSet[production.productionSymbols[j]].find(Symbol(EPSILON_SYMBOL, EPSILON)) == firstSet[production.productionSymbols[j]].end())
                            {
                                break;
                            }

                            // if the next symbol is the last symbol in the production
                            if (j == production.productionSymbols.size() - 1)
                            {
                                // add the follow set of the current non-terminal symbol to the follow set of the next symbol
                                followMap[production.productionSymbols[i]].insert(followMap[rule.first].begin(), followMap[rule.first].end());
                            }
                        }
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
                }
            }
        }
    }

    return dependenciesMap;
}

void ParsingTableBuilder::constructParsingTable(std::map<Symbol, std::vector<Production>> rules,
                                                std::map<Symbol, std::set<Symbol>> first,
                                                std::map<Symbol, std::set<Symbol>> follow)
{
    for (auto &rule : rules) {
        std::vector<Production> productions = rule.second;
        for(auto &production : productions) {
            std::vector<Symbol> symbols = production.productionSymbols;
            if(hasEpsilon(first[symbols[0]])){
                // add each symbol in the Follow set
                for(auto &terminal : follow[rule.first]){
                    if(parsingTable.isEmpty(rule.first, terminal)){
                        std::cout << "Grammar is not LL(1)" << std::endl;
                        exit(0);
                    }
                    parsingTable.addProduction(rule.first, terminal, production);
                }
            } else {
                // add each symbol in the First set
                for(auto &terminal : first[symbols[0]]){
                    if(parsingTable.isEmpty(rule.first, terminal)){
                        std::cout << "Grammar is not LL(1)" << std::endl;
                        exit(0);
                    }
                    parsingTable.addProduction(rule.first, terminal, production);
                }
            }
        }
    }
    addSync(follow);
}

void ParsingTableBuilder::addSync(std::map<Symbol, std::set<Symbol>> follow) {
    for (auto &nonTerminal : follow) {
        for (auto &terminal : nonTerminal.second) {
            if (parsingTable.isEmpty(nonTerminal.first, terminal)) {
                if (parsingTable.getProduction(nonTerminal.first, terminal).productionSymbols[0].type != EPSILON) {
                    std::cout << "Grammar is not LL(1)" << std::endl;
                    exit(0);
                }
                continue;
            }
            parsingTable.getProduction(nonTerminal.first, terminal).productionSymbols.push_back(Symbol("sync", TERMINAL));
        }
    }
}

bool ParsingTableBuilder::hasEpsilon(std::set<Symbol> symbols) {
    return std::find(symbols.begin(), symbols.end(), Symbol("EPSILON", EPSILON)) != symbols.end();
}
