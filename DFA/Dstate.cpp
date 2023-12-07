


#include "Dstate.h"

int Dstate::id_generator = 0;

Dstate::Dstate(set<State*> U)
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

Dstate::Dstate()
        : id(-1), is_final(false){

}

Dstate::Dstate(int id)
        : id(id), is_final(false){

}

 int Dstate::get_id()
{
    return id;
}

 bool Dstate::get_is_final()
{
    return is_final;
}

string Dstate::get_type()
{
        return type;
}

int Dstate::get_priority()
{
        return priority;
}

set<State*> Dstate::get_U()
{
        return U;
}

void Dstate::set_final(bool is_final) {
    this->is_final = is_final;
}



void Dstate::set_priority(int p) {
    this->priority = p ;
}

void Dstate::set_type(string type){
    this->type = type;
}

void Dstate::set_transation(char input, Dstate *to) {
    transitions[input] = to;
}

Dstate* Dstate::get_transation(char input) {
    if(transitions.find(input) != transitions.end())
        return transitions[input];
    return new Dstate();
}
