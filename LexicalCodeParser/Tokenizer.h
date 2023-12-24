#ifndef COMPILER_TOKENIZER_H
#define COMPILER_TOKENIZER_H

#include "../DFA/DFA.h"
#include "./CodeParser.h"
#include "../DFA/Dstate.h"
#include "../OutputWritter/OutputWritter.h"

struct TokenWithLexeme {
    std::string type;
    std::string lexeme;

    TokenWithLexeme() {}

    TokenWithLexeme(std::string type, std::string lexeme) {
        this->type = type;
        this->lexeme = lexeme;
    }

    bool operator==(const TokenWithLexeme &input) const {
        return type == input.type && lexeme == input.lexeme;
    }
};

class Tokenizer {
public:
    Tokenizer(set<Dstate *> minStates, std::string codeFile, OutputWritter *writer);
    struct TokenWithLexeme getNextToken();
    bool match(Dstate* state, int startChar);
    bool hasTokens();
    bool tokenFound();
    void dropOneChar(int startIndex);

private:
    OutputWritter *fwriter;
    Dstate *initialState;
    CodeParser *parser;
    struct TokenWithLexeme tokenWithLexeme;
};
#endif //COMPILER_TOKENIZER_H
