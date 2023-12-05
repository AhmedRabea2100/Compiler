

#include "DFA.h"
#include "state.h"
#include <stack>
#include <queue>
#include <algorithm>


// Function to perform epsilon closure recursively starting from a given state
void DFA::e_closure_recursive(State *s, set<State*>& visited) {
    // Mark the current state as visited
    visited.insert(s);

    // Add the current state to the epsilon closure set
    closure.insert(s);

    // Get all transitions from the current state
    const vector<Transition>& all_transition = s->get_transitions();

    // Iterate through each transition
    for (Transition transition : all_transition) {
        // Check if the transition is an epsilon transition
        if (transition.get_input() == EPSILON) {
            // Retrieve the destination state of the epsilon transition
            auto* to = const_cast<State*>(transition.get_to());

            if (visited.find(to) == visited.end()) {
                // Add the destination state to the epsilon closure set
                closure.insert(to);

                // Recursively perform epsilon closure on the destination state
                e_closure_recursive(to, visited);
            }
        }
    }
}

// Function to calculate the epsilon closure of a given state
set<State*> DFA::e_closure(State* s) {
    // Clear the epsilon closure set
    closure.clear();

    // Set to keep track of visited states during epsilon closure calculation
    set<State*> visited;

    // Perform epsilon closure starting from the given state
    e_closure_recursive(s, visited);

    // Return the epsilon closure set
    return closure;
}


// Function to calculate the epsilon closure of a set of states
set<State*> DFA::e_closure(set<State*> T) {
    // Set to store the epsilon closure of the input set of states
    set<State*> T_closure;

    // Stack to perform depth-first search during epsilon closure calculation
    stack<State*> stack;

    // Push all states in T onto the stack
    for (State* s : T) {
        stack.push(s);
    }

    // Perform depth-first search to compute epsilon closure
    while (!stack.empty()) {
        // Pop the top state from the stack
        State* state = stack.top();
        stack.pop();

        // Calculate epsilon closure for the current state
        set<State*> epsilon_closure_set = e_closure(state);

        // Check if each state in the epsilon closure set is already in T_closure
        for (State* s : epsilon_closure_set) {
            // If the state is not in T_closure, add it and push it onto the stack
            if (T_closure.find(s) == T_closure.end()) {
                T_closure.insert(s);
                stack.push(s);
            }
        }
    }

    // Return the final epsilon closure set for the input set of states
    return T_closure;
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


set<Dstates*> DFA::NFA_to_DFA(set<State*> start_states) {
    queue<Dstates*> unmarked;
    set<Dstates*> dfa_states;

    // Create the initial Dstate
    Dstates* initial = new Dstates(e_closure(start_states));
    dfa_states.insert(initial);
    unmarked.push(initial);

    set<char> inputs = {'0', '1'};

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
