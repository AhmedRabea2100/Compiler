#include "State.h"

int State::max_id = -1;

State::State()
    : id(++max_id), is_final(false), type(""), priority(0)
{}

State::State(bool is_final, string type, int priority)
    : id(++max_id), is_final(is_final), type(type), priority(priority)
{}

const int State::get_id() const
{
    return id;
}

bool State::get_is_final() const
{
    return is_final;
}

void State::set_is_final(bool is_final)
{
    this->is_final = is_final;
}

int State::get_priority() const
{
    return priority;
}

void State::set_priority(int priority)
{
    this->priority = priority;
}

string State::get_type() const
{
    return type;
}

void State::set_type(string type)
{
    this->type = type;
}

vector<Transition> State::get_transitions() const
{
    return transitions;
}

void State::add_transition(Transition t)
{
    transitions.push_back(t);
}

void State::add_transition(State *to, char input)
{
    transitions.push_back(Transition(this, to, input));
}

void State::add_transition(State *to)
{
    transitions.push_back(Transition(this, to));
}
