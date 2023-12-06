#include "transition.h"
#include "state.h"

Transition::Transition(State *from, State *to, char input)
    : from(from), to(to), input(input)
{}

Transition::Transition(State *from, State *to)
    : from(from), to(to), input(EPSILON)
{}

 State *Transition::get_from()
{
    return from;
}

 State *Transition::get_to()
{
    return to;
}

 char Transition::get_input()
{
    return input;
}