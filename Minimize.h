#ifndef COMPILER_MINIMIZE_H
#define COMPILER_MINIMIZE_H

#include "Dstates.h"
#include <set>

class Minimize {

public:
    static set<Dstates *> minimize(set<Dstates *> &dfa);

protected:

private:
    static Dstates* prepare_phai(Dstates* state);

};

#endif //COMPILER_MINIMIZE_H

