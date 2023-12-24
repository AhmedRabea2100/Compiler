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
    CodeParser(std::string name);
    std::vector<char> parseFile();
    int getCurIndex();
    void setCurIndex(int i);
    bool hasChars();
    char nextChar();
    std::string getSubString(int startIndex, int lastIndex);

private:
    int curIndex = 0;
    std::string fileName;
    std::vector<char> fileChars;
};

#endif //COMPILER_CODEPARSER_H
