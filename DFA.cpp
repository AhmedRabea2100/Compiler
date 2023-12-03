//
// Created by User on 12/3/2023.
//

#include "DFA.h"
#include "state.h"
#include <stack>

void DFA::e_closure_recursive(State *s,set<State*>& visited) {
    visited.insert(s);
    closure.insert(s);
    const vector<Transition>& all_transition = s->get_transitions();

    for (Transition transition : all_transition)
    {
        if (transition.get_input() == EPSILON)
        {
            auto* to = const_cast<State*>(transition.get_to());
            closure.insert(to);
            e_closure_recursive(to, visited);
        }
    }

}

set<State*> DFA::e_closure(State* s)
{
    closure.clear();
    set<State*> visited;

    e_closure_recursive(s,visited);
    return closure;
}

set<State*> DFA::e_closure(set<State*> T)
{
    set<State*> T_closure;
    // push all states in T onto stack
    stack<State*> stack;
    for (State* s : T)
    {
        stack.push(s);
    }
    while (!stack.empty()){
        State* state = stack.top();
        stack.pop();
        set<State*> set = e_closure(state);
        // check if set is already in T
        for (State* s : set)
        {
            if (T_closure.find(s) == T_closure.end())
            {
                T_closure.insert(s);
                stack.push(s);
            }
        }
    }
    return T_closure;
}

set<State*> DFA::move(set<State*> T, char input)
{
    set<State*> T_move;
    for (State* s : T)
    {
        const vector<Transition>& all_transition = s->get_transitions();
        for (Transition transition : all_transition)
        {
            if (transition.get_input() == input)
            {
                auto* to = const_cast<State*>(transition.get_to());
                T_move.insert(to);
            }
        }
    }
    return T_move;

}