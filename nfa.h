#include "state.h"

class NFA
{
public:
    NFA();
    NFA(State &start, State &end);
    NFA(char input);

    static NFA uni(NFA &a, NFA &b);
    static NFA concat(NFA &a, NFA &b);
    static NFA star(NFA &a);
    static NFA plus(NFA &a);

    State get_start();
    State get_end();

private:
    State start;
    State end;
};
