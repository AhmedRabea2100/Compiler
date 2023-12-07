#ifndef COMPILER_MINIMIZE_H
#define COMPILER_MINIMIZE_H

#include "../DFA/Dstate.h"
#include <set>

class Minimize {

public:
    static set<Dstate *> minimize(set<Dstate *> &dfa);

protected:

private:
    static Dstate* prepare_phai(Dstate* state);

};

#endif //COMPILER_MINIMIZE_H

