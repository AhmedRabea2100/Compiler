#pragma once
#include "State.h"

class NFA
{
public:
    State start;
    State end;

    NFA();
    NFA(State &start, State &end);
    NFA(char input);

    static NFA* uni(NFA *a, NFA *b);
    static NFA* concat(NFA *a, NFA *b);
    static NFA* star(NFA *a);
    static NFA* plus(NFA *a);
};
