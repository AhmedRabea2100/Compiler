#ifndef COMPILER_MINIMIZE_H
#define COMPILER_MINIMIZE_H

#include "Dstates.h"
#include <set>

class Minimize {

public:
    static set<Dstates*> minimize(const set<Dstates*>& dfa) ;
protected:

private:

};

#endif //COMPILER_MINIMIZE_H

