#ifndef COMPILER_RESULT_H
#define COMPILER_RESULT_H

#include <string>
#include "../ContextFreeGrammarParser/CFGParser.h"

struct ParseResult {
    std::string msg;
    std::pair<Symbol, Production> rule;
    bool tokenDone;

    ParseResult() {
        msg = "";
    }
};

#endif //COMPILER_RESULT_H
