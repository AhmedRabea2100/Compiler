#include "NFA.h"

NFA::NFA() {}

NFA::NFA(State &start, State &end) : start(start), end(end) {}

NFA::NFA(char input)
{
    start.add_transition(&end, input);
}

NFA* NFA::uni(NFA *a, NFA *b)
{
    NFA* result = new NFA();
    result->start.add_transition(&a->start);
    result->start.add_transition(&b->start);

    a->end.add_transition(&result->end);
    b->end.add_transition(&result->end);
    return result;
}

NFA* NFA::concat(NFA *a, NFA *b)
{
    NFA* result = new NFA();
    result->start.add_transition(&a->start);
    a->end.add_transition(&b->start);
    b->end.add_transition(&result->end);
    return result;
}

NFA* NFA::star(NFA *a)
{
    NFA* result = new NFA();
    result->start.add_transition(&a->start);
    a->end.add_transition(&result->end);
    result->start.add_transition(&result->end);
    a->end.add_transition(&a->start);
    return result;
}

NFA* NFA::plus(NFA *a)
{
    NFA* result = new NFA();
    result->start.add_transition(&a->start);
    a->end.add_transition(&result->end);
    a->end.add_transition(&a->start);
    return result;
}
