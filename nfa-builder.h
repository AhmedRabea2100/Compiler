#pragma once
#include <vector>
#include "state.h"
#include <unordered_set>
#include "nfa.h"
#include "./GrammarParser/Token.h"
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
