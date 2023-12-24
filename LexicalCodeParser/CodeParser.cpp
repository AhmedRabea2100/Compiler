#include "CodeParser.h"

CodeParser::CodeParser(std::string name): fileName(name){}

std::vector<char> CodeParser::parseFile() {
    std::ifstream file(fileName);
    char c;
    while (file.get(c))
        fileChars.push_back(c);
    file.close();
    return fileChars;
}

int CodeParser::getCurIndex() {
    return curIndex;
}

void CodeParser::setCurIndex(int i) {
    curIndex = i;
}

char CodeParser::nextChar() {
    return fileChars[curIndex++];
}

bool CodeParser::hasChars() {
    return curIndex != fileChars.size();
}

std::string CodeParser::getSubString(int startIndex, int lastIndex) {
    std::string subString;
    for (int i = startIndex; i < lastIndex; i++) {
        subString += fileChars[i];
    }
    return subString;
}