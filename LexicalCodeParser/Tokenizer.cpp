#include "Tokenizer.h"

Tokenizer::Tokenizer(set<Dstate *> minStates, std::string codeFile, OutputWritter *writer) {
    fwriter = writer;
    parser = new CodeParser(codeFile);
    parser->parseFile();

    // get initial state
    for (auto &state: minStates) {
        if (state->get_id() == 0) {
            initialState = state;
            break;
        }
    }
}

struct TokenWithLexeme Tokenizer::getNextToken() {
    int curIndex = parser->getCurIndex();
    if (!match(initialState, curIndex)) {
        fwriter->writeError("ERROR: " + parser->getSubString(curIndex, curIndex + 1));
        dropOneChar(curIndex + 1);
    }
    return tokenWithLexeme;
}

bool Tokenizer::match(Dstate* state, int startChar) {
    Dstate *currentState = state, *nextState = nullptr;
    char c;
    std::string currentMatchType = "";
    std::string currentMatch = "";
    int currentMatchIndex = -1;

    tokenWithLexeme.type = "", tokenWithLexeme.lexeme = "";
    while(parser->hasChars()) {
        c = parser->nextChar();
        if (currentState->transitions.find(c) == currentState->transitions.end() || iswspace(c) || currentState->get_id() == -1)
            break;
        nextState = currentState->transitions[c];
        if (nextState->get_is_final()) {
            currentMatchIndex = parser->getCurIndex();
            currentMatch = parser->getSubString(startChar, currentMatchIndex);
            currentMatchType = nextState->get_type();
        }
        currentState = nextState;
    }

    if (iswspace(c) && currentMatchIndex == -1)
        return true;
    else if (currentMatchIndex == -1)
        return false;
    else {
        parser->setCurIndex(currentMatchIndex);
        fwriter->writeMatch(currentMatch, currentMatchType);
        tokenWithLexeme.type = currentMatchType;
        tokenWithLexeme.lexeme = currentMatch;
    }
    return true;
}

bool Tokenizer::hasTokens() {
    return parser->hasChars();
}

bool Tokenizer::tokenFound() {
    return tokenWithLexeme.type != "";
}

void Tokenizer::dropOneChar(int startIndex) {
    parser->setCurIndex(startIndex);
}