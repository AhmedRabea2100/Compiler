#include "state.h"

int State::max_id = -1;

State::State(bool is_final, Type type, int priority)
    : id(++max_id), is_final(is_final), type(type), priority(priority)
{}

const int State::get_id()
{
    return id;
}

const bool State::get_is_final()
{
    return is_final;
}

const State::Type State::get_type()
{
    return type;
}

const int State::get_priority()
{
    return priority;
}

vector<Transition> State::get_transitions()
{
    return transitions;
}

void State::add_transition(Transition t)
{
    transitions.push_back(t);
}
