#include <iostream>
#include "DFA.h"
#include <stack>
#include <queue>
#include "../NFA/NFABuilder.h"
#include <algorithm>
#include "Dstate.h"

set<State *> returned;

// Function to calculate the epsilon closure of a state
void DFA::e_closure(State *start)
{
    returned.insert(start);
    for (auto &transition : start->get_transitions())
    {
        if (transition.get_input() == EPSILON)
        {
            if (returned.find(transition.get_to()) == returned.end())
            {
                e_closure(transition.get_to());
            }
        }
    }
}

// Function to calculate the epsilon closure of a set of states
set<State *> DFA::e_closure(set<State *> T)
{
    set<State *> returned_set;
    for (auto &state : T)
    {
        returned.clear();
        e_closure(state);
        set<State *> s = returned;
        returned_set.insert(s.begin(), s.end());
    }
    return returned_set;
}

// Function to calculate the move of a set of states given a specific input character
set<State *> DFA::move(set<State *> T, char input)
{
    set<State *> T_move;
    for (State *s : T)
    {
        // Get all transitions from the current state
        const vector<Transition> &all_transitions = s->get_transitions();
        for (Transition transition : all_transitions)
        {
            if (transition.get_input() == input)
            {
                auto *to = const_cast<State *>(transition.get_to());

                // Add the destination state to the set T_move
                T_move.insert(to);
            }
        }
    }
    return T_move;
}

set<Dstate *> DFA::NFA_to_DFA(set<State *> start_states, unordered_set<char> inputs)
{
    queue<Dstate *> unmarked;
    vector<Dstate *> dfa_states;
    map<set<State *>, Dstate *> set_of_states;

    // Create the initial Dstate
    Dstate *initial = new Dstate(e_closure(start_states));
    dfa_states.push_back(initial);
    unmarked.push(initial);

    while (!unmarked.empty())
    {
        Dstate *T = unmarked.front();
        unmarked.pop();

        for (const auto &input : inputs)
        {
            set<State *> U = e_closure(move(T->get_U(), input));
            if (U.empty())
            {
                T->set_transation(input, new Dstate());
                continue;
            }

            if (set_of_states.find(U) == set_of_states.end())
            {
                // U not found, create a new Dstates
                Dstate *dest = new Dstate(U);
                dfa_states.push_back(dest);
                unmarked.push(dest);
                T->set_transation(input, dest);
                set_of_states[U] = dest;
            }
            else
            {
                // U found, set the transition to the existing Dstates
                T->set_transation(input, set_of_states[U]);
            }
        }
    }
    set<Dstate *> dfa_states_set = set<Dstate *>(dfa_states.begin(), dfa_states.end());

    return dfa_states_set;
}
