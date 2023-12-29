#include "CFGParser.h"

#define START_DETECTOR '#'
#define TERMINAL_DETECTOR '\''
#define OR  '|'

const std::string PRODUCTION_DETECTOR = "::=";
std::map<Symbol, std::vector<Production>> rules;
Symbol startSymbol("", START);

std::map<Symbol, std::vector<Production>> CFGParser::getRules(std::string grammarFilePath) {
    std::ifstream grammarFile(grammarFilePath);
    std::string line, rule;
    while (std::getline(grammarFile, line)) {
        if (line.empty())
            continue;
        if (line[0] == START_DETECTOR && !rule.empty()){ // Found a new rule
            resolveRule(rule);
            rule.clear();
        }
        rule += line;
    }

    if(!rule.empty()) {
        resolveRule(rule);
        rule.clear();
    }

    eliminateLeftRecursion();
    eliminateLeftFactoring();

    grammarFile.close();

    return rules;
}

void CFGParser::eliminateLeftFactoring() {
    std::map<Symbol,std::vector<Production>> temp;

    // Check for left factoring
    for (auto it = rules.begin(); it != rules.end(); it++)
        checkLeftFactoring(temp, it);

    // Replace the old rules with the new ones
    for(auto &x: temp){
        auto idx = rules.find(x.first);
        if(idx != rules.end()){
            idx->second.clear();
            idx->second = x.second;
        }
        else{
            if(!x.second.empty())
                rules[x.first] = x.second;
        }
    }
}

void CFGParser::checkLeftFactoring(std::map<Symbol, std::vector<Production>> &temp, std::map<Symbol, std::vector<Production>>::iterator idx) {
    std::vector<Production> productions = idx->second;
    std::map<Symbol, std::vector<Production>> sameStartSymbolProductions;

    for(auto &x: productions){
        Symbol firstSymbol = x.productionSymbols[0];
        auto it = sameStartSymbolProductions.find(firstSymbol);

        if(it != sameStartSymbolProductions.end())// Seen before
            it->second.push_back(x);
        else{ // Never seen before
            std::vector<Production> newProduction;
            newProduction.push_back(x);
            sameStartSymbolProductions[firstSymbol] = newProduction;
        }
    }

    // Found Left Factoring
    if(sameStartSymbolProductions.size() < productions.size()){
        idx->second.clear();
        for (auto it = sameStartSymbolProductions.begin(); it != sameStartSymbolProductions.end(); it++){
            auto returnLeft = findLongestLeftFactor(temp, idx->first, it->second);

            if (returnLeft != temp.end() && returnLeft->second.size() > 1)
                checkLeftFactoring(temp, returnLeft);
        }
    }
}

std::map <Symbol, std::vector<Production>>::iterator CFGParser::findLongestLeftFactor(
        std::map<Symbol, std::vector<Production>> &temp, Symbol lhs, std::vector<Production> productions) {
    bool notMatched = false;
    int idx = 1;

    // Find max prefix of all productions
    while(idx < productions[0].productionSymbols.size()) {
        Symbol firstSymbol = productions[0].productionSymbols[idx];

        for (int i = 1; i < productions.size(); i++) {
            if (idx >= productions[i].productionSymbols.size() || productions[i].productionSymbols[idx] != firstSymbol) {
                notMatched = true;
                break;
            }
        }

        if (notMatched) {
            idx--;
            break;
        }
        idx++;
    }

    if(idx == 1 && idx >= productions[0].productionSymbols.size() || idx == productions[0].productionSymbols.size())
        idx--;

    // The common prefix between the productions
    Production prefixProduction;
    for(int i = 0; i <= idx; i++)
        prefixProduction.productionSymbols.push_back(productions[0].productionSymbols[i]);

    // Create the new rules
    std::vector<Production> newProductions;
    int minLength = 1e9;
    bool hasMore = false;

    for(auto &x: productions){
        Production newRuleProduction;

        for (int j = idx + 1; j < x.productionSymbols.size(); j++)
            newRuleProduction.productionSymbols.push_back(x.productionSymbols[j]);

        // Not epsilon
        if (!newRuleProduction.productionSymbols.empty()) {
            hasMore = true;
            newProductions.push_back(newRuleProduction);
        }
        minLength = std::min(minLength, (int)x.productionSymbols.size());
    }

    std::string newRuleName = generateNewRule(temp, lhs.name);
    Symbol newNonTerminal(newRuleName, NON_TERMINAL);

    if(hasMore){
        // Add A' to end of A productions
        prefixProduction.productionSymbols.push_back(newNonTerminal);

        // Add Epsilon to A' productions
        if(minLength == idx + 1){
            Production epsilonProduction;
            epsilonProduction.productionSymbols.push_back(Symbol(EPSILON_SYMBOL, EPSILON));
            newProductions.push_back(epsilonProduction);
        }
    }

    // Check if it doesn't exist before
    auto tempIdx = temp.find(lhs);
    if (tempIdx != temp.end())
        temp.find(lhs)->second.push_back(prefixProduction);
    else {
        std::vector<Production> newP;
        newP.push_back(prefixProduction);
        temp[lhs] = newP;
    }

    if (!newProductions.empty()) {
        return temp.insert(std::pair<Symbol, std::vector<Production>>(newNonTerminal, newProductions)).first;
    }
    else {
        return temp.end();
    }

}

void CFGParser::eliminateLeftRecursion() {
    std::set<Symbol> nonTerminals;
    std::map<Symbol, std::vector<Production>> temp;

    for(auto &rule: rules) {
        Symbol nonTerminal = rule.first;
        std::vector<Production> productions = rule.second;

        bool isLeftRecursive = false;
        std::vector<Production> beta;
        std::vector<Production> alpha;
        for (auto currProduction: productions) {
            Symbol firstSymbolInProduction = currProduction.productionSymbols[0];
            auto idx = nonTerminals.find(firstSymbolInProduction);

            if (idx != nonTerminals.end()) { // Non-Immediate Left Recursion
                std::vector<Production> secondNonTerminalProductions = rules.find(*idx)->second;
                for (auto &secondNonTerminalProduction: secondNonTerminalProductions) {
                    Symbol firstSymbolInSecondProduction = secondNonTerminalProduction.productionSymbols[0];
                    Production concatProduction = secondNonTerminalProduction;
                    for (int i = 1; i < currProduction.productionSymbols.size(); i++) {
                        concatProduction.productionSymbols.push_back(currProduction.productionSymbols[i]);
                    }
                    isLeftRecursive |= checkLeftRecursion(nonTerminal, firstSymbolInSecondProduction, concatProduction,
                                                          alpha, beta);
                }
            }
            else { // Immediate Left Recursion
                isLeftRecursive |= checkLeftRecursion(nonTerminal, firstSymbolInProduction, currProduction, alpha,beta);
            }
        }

        if(isLeftRecursive){
            std::string newRuleName = generateNewRule(temp, nonTerminal.name);
            Symbol newRule(newRuleName, NON_TERMINAL);

            // Create the new splitted rules
            for(auto &x: beta)
                x.productionSymbols.push_back(newRule);
            for(auto &x: alpha)
                x.productionSymbols.push_back(newRule);

            // Epsilion production for rule'
            Production epsilonProduction;
            epsilonProduction.productionSymbols.push_back(Symbol(EPSILON_SYMBOL, EPSILON));
            alpha.push_back(epsilonProduction);

            // Add the new productions to original rule
            rule.second.clear();
            for(auto &x: beta)
                rule.second.push_back(x);

            temp[newRule] = alpha;
        }
        nonTerminals.insert(nonTerminal);
    }

    for(auto &x: temp)
        rules[x.first] = x.second;
}

bool CFGParser::checkLeftRecursion(Symbol lhs, Symbol rhs, Production p, std::vector<Production> &alpha, std::vector<Production> &beta){
    if (lhs == rhs) {
        Production pr;
        for (int i = 1; i < p.productionSymbols.size(); i++)
            pr.productionSymbols.push_back(p.productionSymbols[i]);
        alpha.push_back(pr);
        return true;
    }
    else {
        beta.push_back(p);
        return false;
    }
}

std::string CFGParser::generateNewRule(std::map<Symbol, std::vector<Production>> &temp, std::string lhs) {
    int add = 0;
    Symbol newLHS(lhs, NON_TERMINAL);
    std::string newNonTerminalName;
    do {
        newNonTerminalName = lhs;
        newNonTerminalName.append(std::to_string(add));
        newLHS.name = newNonTerminalName;
        add++;
    } while (rules.find(newLHS) != rules.end() || temp.find(newLHS) != temp.end());
    return newNonTerminalName;
}

void  CFGParser::resolveRule(std::string rule){
    rule = rule.erase(0, 1);
    std::vector<std::string> ruleParts = split(rule, PRODUCTION_DETECTOR);
    std::string lhs = trim(ruleParts[0], " \t");
    std::string rhs = trim(ruleParts[1], " \t");
    std::vector<Production> productions = rhsToProductions(rhs);
    if(rules.empty()) {
        startSymbol.name = lhs;
        rules[startSymbol] = productions;
    }
    else
        rules[Symbol(lhs, NON_TERMINAL)] = productions;
}

std::vector<Production> CFGParser::rhsToProductions(std::string rhs) {
    std::vector<Production> productions;
    Production p;
    productions.push_back(p);

    bool isNonTerminal = false;
    char currentChar;
    std::string currentProductionSymbol;

    for(int i = 0; i < rhs.length(); i++){
        currentChar = rhs[i];
        if(iswspace(currentChar)){ // Found space
            if(!currentProductionSymbol.empty()) {
                if (currentProductionSymbol == startSymbol.name)
                    productions.back().productionSymbols.push_back(Symbol(currentProductionSymbol, START));
                else
                    productions.back().productionSymbols.push_back(Symbol(currentProductionSymbol, NON_TERMINAL));
                currentProductionSymbol.clear();
            }
        }
        else if(currentChar == TERMINAL_DETECTOR && (i == 0 || (i > 0 && rhs[i - 1] != '\\'))){ // Found terminal
            if(!isNonTerminal)
                isNonTerminal = true;
            else{
                if(!currentProductionSymbol.empty()) {
                    productions.back().productionSymbols.push_back(Symbol(currentProductionSymbol, TERMINAL));
                    currentProductionSymbol.clear();
                    isNonTerminal = false;
                }
            }
        }
        else if(currentChar == OR && (i == 0 || (i > 0 && rhs[i - 1] != '\\'))){ // Found or separator
            if (!currentProductionSymbol.empty()) {
                if (currentProductionSymbol == startSymbol.name)
                    productions.back().productionSymbols.push_back(Symbol(currentProductionSymbol, START));
                else
                    productions.back().productionSymbols.push_back(Symbol(currentProductionSymbol, NON_TERMINAL));
                currentProductionSymbol.clear();
            }
            Production p;
            productions.push_back(p);
        }
        else if (currentChar == 'L' && i > 0 && rhs.at(i - 1) == '\\' && (i < 2 || rhs.at(i - 2) != '\\')) {
            currentProductionSymbol.append(1, currentChar);
            Symbol newTerminal(currentProductionSymbol, EPSILON);
            productions.back().productionSymbols.push_back(newTerminal);
            currentProductionSymbol.clear();
        }
        else{
            currentProductionSymbol += currentChar;
        }
    }

    if (!currentProductionSymbol.empty()) {
        if (currentProductionSymbol == startSymbol.name)
            productions.back().productionSymbols.push_back(Symbol(currentProductionSymbol, START));
        else
            productions.back().productionSymbols.push_back(Symbol(currentProductionSymbol, NON_TERMINAL));
        currentProductionSymbol.clear();
    }

    return productions;
}

std::vector<std::string> CFGParser::split(std::string str, std::string delimiter) {
    std::vector<std::string> tokens;
    std::size_t prevFound = 0, found = str.find(delimiter);

    while (found != std::string::npos) {
        if (found == 0 || (str.at(found - 1) != '\\')) {
            std::string token = str.substr(prevFound, found - prevFound);
            token = trim(token, " \t");
            if (!token.empty())
                tokens.push_back(token);
        }
        prevFound = found + delimiter.length();
        found = str.find(delimiter, prevFound);
    }

    std::string token = str.substr(prevFound, str.length());
    token = trim(token, " \t");

    if (!token.empty())
        tokens.push_back(token);

    return tokens;
}

std::string CFGParser::trim(std::string str, std::string whitespace) {
    size_t start = str.find_first_not_of(whitespace);
    size_t end = str.find_last_not_of(whitespace);
    if (start == std::string::npos)
        return "";
    return str.substr(start, end - start + 1);
}