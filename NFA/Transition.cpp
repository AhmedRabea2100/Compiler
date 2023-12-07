#include "Transition.h"
#include "State.h"

Transition::Transition(State *from, State *to, char input)
    : from(from), to(to), input(input)
{}

Transition::Transition(State *from, State *to)
    : from(from), to(to), input(EPSILON)
{}

const State *Transition::get_from()
{
    return from;
}

const State *Transition::get_to()
{
    return to;
}

const char Transition::get_input()
{
    return input;
}
