#ifndef COMPILER_CODEPARSER_H
#define COMPILER_CODEPARSER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "../DFA/Dstate.h"
#include <unordered_map>
class CodeParser {
public:
    CodeParser(set<Dstate*> states);
    std::vector<char> parseFile(std::string fileName);

private:
    static bool match(Dstate *state, int index);
    std::set<Dstate*> minStates;
};

#endif //COMPILER_CODEPARSER_H
