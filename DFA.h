//
// Created by User on 12/3/2023.
//

#ifndef COMPILER_DFA_H
#define COMPILER_DFA_H

#endif //COMPILER_DFA_H

#include "state.h"
#include "transition.h"
#include <vector>
#include <map>
#include <set>

using namespace std;

class DFA
{
public:
    set<State*> e_closure(State* s);
    set<State*> e_closure(set<State*> T);
    set<State*> move(set<State*> T, char input);
protected:

private:
    set<State*> closure;
    void e_closure_recursive(State *s, set<State *> &visited);
};
