#ifndef COMPILER_DFA_H
#define COMPILER_DFA_H

#include "state.h"
#include "transition.h"
#include "Dstates.h"
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

using namespace std;

class DFA
{
public:
    set<State*> e_closure(State* s);
    set<State*> e_closure(set<State*> T);
    set<State*> move(set<State*> T, char input);
    set<Dstates*> NFA_to_DFA(set<State*> start_states,unordered_set<char> inputs);
protected:

private:
    set<State*> closure;
    void e_closure_recursive(State *s, set<State *> &visited);


};



#endif //COMPILER_DFA_H

