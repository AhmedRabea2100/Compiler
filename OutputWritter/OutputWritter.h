#pragma once
#include <string>
#include <set>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include "../DFA/Dstate.h"
using namespace std;

class OutputWritter
{
public:
    OutputWritter(std::string fileName);
    void closeFile();
    void writeMatch(std::string match, std::string tokenClass);
    void writeError(std::string error);
    void writeTransitionTable(set<Dstate *> minimized_dfa, unordered_set<char> inputs);
private:
    ofstream file;
    void fillSpaces(int spaceNum);
    void drawLine(int length);
};
