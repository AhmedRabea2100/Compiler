#pragma once
#include <string>
#include <set>
#include <unordered_set>
#include <fstream>
#include "../DFA/Dstate.h"
using namespace std;

class TransitionTableWritter
{
public:
    void write(set<Dstate *> minimized_dfa, unordered_set<char> inputs);

private:
    ofstream file;
    void fillSpaces(int spaceNum);
    void drawLine(int length);
};
