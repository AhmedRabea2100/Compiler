#ifndef COMPILER_DFA_H
#define COMPILER_DFA_H

#include "../NFA/State.h"
#include "../NFA/Transition.h"
#include "Dstate.h"
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

using namespace std;

class DFA
{
public:
    void e_closure(State *s);
    set<State *> e_closure(set<State *> T);
    set<State *> move(set<State *> T, char input);
    set<Dstate *> NFA_to_DFA(set<State *> start_states, unordered_set<char> inputs);
};

#endif // COMPILER_DFA_H
