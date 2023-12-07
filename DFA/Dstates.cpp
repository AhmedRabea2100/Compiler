


#include "Dstates.h"

int Dstates::id_generator = 0;

Dstates::Dstates(set<State*> U)
        : id(id_generator++), is_final(false), type(""), priority(-1)
{
    this->U = U;
    // Flag to track whether an accepting state has been encountered
    bool flag = false;

    for (State* state : this->U) {
        // Check if the state is an accepting state
        if (state->get_is_final()) {
            // If it's the first accepting state encountered, or it has higher priority
            if (this->priority < state->get_priority()) {
                // Update the type and priority
                this->type = state->get_type();
                this->priority = state->get_priority();
            }

            flag = true;
        }
    }

    if (U.empty()) {
        this->type = "";
    }

    // Set is_final based on whether at least one accepting state was found
    this->is_final = flag;

}

Dstates::Dstates()
        : id(-1), is_final(false){

}

 int Dstates::get_id() const
{
    return id;
}

 bool Dstates::get_is_final() const
{
    return is_final;
}

string Dstates::get_type()
{
        return type;
}

int Dstates::get_priority() const
{
        return priority;
}

set<State*> Dstates::get_U()
{
        return U;
}

void Dstates::set_transation(char input, Dstates *to) {
    transitions[input] = to;
}

Dstates* Dstates::get_transation(char input) {
    if(transitions.find(input) != transitions.end())
        return transitions[input];
    return new Dstates();
}
