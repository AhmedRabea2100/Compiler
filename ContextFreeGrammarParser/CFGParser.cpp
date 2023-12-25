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

    std::cout << "=========================================" << std::endl;
    for(auto &x: rules){
        std::cout << x.first.name << " -> ";
        for(auto &y: x.second){
            for(auto &z: y.productionSymbols){
                std::cout << z.name << " " << z.type << " ";
            }
            std::cout << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    eliminateLeftRecursion();
//    executeLeftFactoring(&rules);

    grammarFile.close();
    return rules;
}

void CFGParser::eliminateLeftRecursion() {
    std::set<Symbol> nonTerminals;
    std::map<Symbol, std::vector<Production>> temp;

    for(auto &rule: rules){
        Symbol nonTerminal = rule.first;
        std::vector<Production> curRules = rule.second;

        bool isLeftRecursive = false;
        std::vector<Production> beta;
        std::vector<Production> alpha;



    }

}

void  CFGParser::resolveRule(std::string rule){
    rule = rule.erase(0, 1);
    std::vector<std::string> ruleParts = split(rule, PRODUCTION_DETECTOR);
    std::string lhs = trim(ruleParts[0], " \t");
    std::string rhs = trim(ruleParts[1], " \t");
    if(rules.empty())
        startSymbol.name = lhs;
    std::vector<Production> productions = rhsToProductions(rhs);
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