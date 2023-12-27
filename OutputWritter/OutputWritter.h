#pragma once
#include <string>
#include <set>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include "../DFA/Dstate.h"
#include <list>
#include "../LL1Parser/ParsingTable.h"
#include "../Parser/Result.h"
//#include "../Parser/parser.h"

using namespace std;

class OutputWritter
{
public:
    OutputWritter(std::string fileName);
    void closeFile();
    void writeMatch(std::string match, std::string tokenClass);
    void writeError(std::string error);
    void writeTransitionTable(set<Dstate *> minimized_dfa, unordered_set<char> inputs);

    void writeParsingTable(ParsingTable *parsingTable);

    void writeParserResult(ParseResult& parserResult);

    void writeLeftDerivation(std::list<Symbol> *derivationLeftSide, std::list<Symbol> *stack, std::string errorMsg);
private:
    ofstream file;
    void fillSpaces(int spaceNum);
    void drawLine(int length);
};
