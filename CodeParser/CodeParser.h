#ifndef COMPILER_CODEPARSER_H
#define COMPILER_CODEPARSER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../DFA/Dstates.h"

class CodeParser {
public:
    CodeParser(set<Dstates*> states);
    std::vector<char> parseFile(std::string fileName);

private:
    static bool match(Dstates *state, int index);
    std::set<Dstates*> minStates;
};

#endif //COMPILER_CODEPARSER_H
