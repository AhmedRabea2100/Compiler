#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "State.h"
#include "NFA.h"
#include "../GrammarParser/Token.h"
using namespace std;

class NFABuilder
{
public:
    NFABuilder();
    NFA* build(Token *token);
    NFA build(vector<Token *> tokens);
    NFA get_NFA();
    unordered_set<char> get_inputs();

private:
    State start;
    unordered_set<char> inputs;

    string expand(char c1, char c2);
};
