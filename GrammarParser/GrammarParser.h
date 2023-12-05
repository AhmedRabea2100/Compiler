#ifndef COMPILER_GRAMMARPARSER_H
#define COMPILER_GRAMMARPARSER_H
#include "Token.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <stack>
#include <queue>

class GrammarParser {
public:
    GrammarParser();
    std::vector<Token *> getTokens(std::string grammarFilePath);

private:
    static bool checkReserved(std::string str);
    static void handleReserved(std::string str);
    static bool checkPunctuation(std::string str);
    static void handlePunctuation(std::string str);
    static bool checkRegularExpression(std::string str);
    static void handleRegularExpression(std::string str, int priority);
    static bool checkRegularDefinition(std::string str);
    static void handleRegularDefinition(std::string str);
    static std::vector<RegexChar*> infixToPostfix(std::string str);
    static std::string addConcatenateSymbol(std::string str);
    static std::string replaceVariableWithRegex(std::string str);
    static std::string trim(std::string str, std::string whitespace);
    static std::string removeExtraSpaces(std::string str);
    static std::string replaceSpacesWith(std::string str, std::string replaceWith);
    static struct RegexChar* createRegexChar(char c, Type charType);
};

#endif //COMPILER_GRAMMARPARSER_H
