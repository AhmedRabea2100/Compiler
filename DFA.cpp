

#include "DFA.h"
#include <stack>
#include <queue>
#include "./NFA/NFABuilder.h"
#include <algorithm>
#include <iostream>


//// Function to perform epsilon closure recursively starting from a given state
//void DFA::e_closure_recursive(State *s, set<State*>& visited,set<State*>& closure) {
//    // Mark the current state as visited
//    visited.insert(s);
//
//    // Add the current state to the epsilon closure set
//    closure.insert(s);
//
//    // Get all transitions from the current state
//    const vector<Transition>& all_transition = s->get_transitions();
//
//    // Iterate through each transition
//    for (Transition transition : all_transition) {
//        // Check if the transition is an epsilon transition
//        if (transition.get_input() == EPSILON) {
//            // Retrieve the destination state of the epsilon transition
//            auto* to = const_cast<State*>(transition.get_to());
//            if (visited.find(to) == visited.end()) {
////                cout <<"from: "<<transition.get_from()->get_id() <<"   to: " << to->get_id()<< "  input:  +" << transition.get_input() << "+  is_final "<< to->get_is_final()<< endl;
//                // Add the destination state to the epsilon closure set
//                closure.insert(to);
//
//                // Recursively perform epsilon closure on the destination state
//                e_closure_recursive(to, visited, closure);
//            }
//        }
//    }
//}
//
//
//// Function to calculate the epsilon closure of a given state
//set<State*> DFA::e_closure(State* s) {
//    // Clear the epsilon closure set
//
//    set<State*> closure;
//
//    // Set to keep track of visited states during epsilon closure calculation
//    set<State*> visited;
//
//    // Perform epsilon closure starting from the given state
//    e_closure_recursive(s, visited, closure);
//
//    // Return the epsilon closure set
//    return closure;
//}


set<State*> returned;

void DFA::t(State* start)
{
    returned.insert(start);
    for(auto& transition : start->get_transitions())
    {
        if(transition.get_input() == EPSILON)
        {
            if(returned.find(transition.get_to()) == returned.end())
            {

                t(transition.get_to());
            }
        }
    }
}

// Function to calculate the epsilon closure of a set of states
set<State*> DFA::e_closure(set<State*> T) {
    set<State*> returned_set;
    for (auto& state : T)
    {

        returned.clear();
        t(state);
        set<State*> s = returned;
        returned_set.insert(s.begin(), s.end());
    }
    return returned_set;
}


// Function to calculate the move of a set of states given a specific input character
set<State*> DFA::move(set<State*> T, char input) {
    // Set to store the resulting set of states after applying the move operation
    set<State*> T_move;

    // Iterate through each state in the input set T
    for (State* s : T) {
        // Get all transitions from the current state
        const vector<Transition>& all_transitions = s->get_transitions();

        // Iterate through each transition of the current state
        for (Transition transition : all_transitions) {
            // Check if the transition has the specified input character
            if (transition.get_input() == input) {
                // Retrieve the destination state of the transition
                auto* to = const_cast<State*>(transition.get_to());

                // Add the destination state to the set T_move
                T_move.insert(to);
            }
        }
    }

    // Return the resulting set of states after applying the move operation
    return T_move;
}


set<Dstates*> DFA::NFA_to_DFA(set<State*> start_states,unordered_set<char> inputs) {
    queue<Dstates*> unmarked;
    set<Dstates*> dfa_states;

    // Create the initial Dstate
    Dstates* initial = new Dstates(e_closure(start_states));
    dfa_states.insert(initial);
    unmarked.push(initial);


    while (!unmarked.empty()) {
        Dstates* T = unmarked.front();
        unmarked.pop();

        for (const auto& input : inputs) {
            set<State*> U = e_closure(move(T->get_U(), input));

            if (U.empty()) {
                T->set_transation(input, new Dstates());
                continue;
            }

            // Find U in dfa_states
            auto it = find_if(dfa_states.begin(), dfa_states.end(),
                              [&U](Dstates* d) { return d->get_U() == U; });

            if (it == dfa_states.end()) {
                // U not found, create a new Dstates
                Dstates* dest = new Dstates(U);
                dfa_states.insert(dest);
                unmarked.push(dest);
                T->set_transation(input, dest);
            } else {
                // U found, set the transition to the existing Dstates
                T->set_transation(input, *it);
            }
        }
    }

    return dfa_states;
}
