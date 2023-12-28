#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <vector>
#include <list>
#include "../ContextFreeGrammarParser/CFGParser.h"
#include "../LL1Parser/ParsingTable.h"
#include "../LexicalCodeParser/Tokenizer.h"
#include "../OutputWritter/OutputWritter.h"
#include "Result.h"


class Parser {
public:

    static Parser &getInstance() {
        static Parser parser;
        return parser;
    }

    void parse(ParsingTable *parsingTable,
               Tokenizer *tokenizer,
               OutputWritter *parserWriter,
               OutputWritter *leftDerivationWriter);

private:
    enum ERROR {
        UNMATCHED_TERMINALS, EMPTY_CELL, SYNC_CELL
    };
    const TokenWithLexeme END_LEXEME = TokenWithLexeme(END_SYMBOL, END_SYMBOL);

    std::list<Symbol> *stack;
    ParsingTable *parseTable;
    std::list<Symbol> *derivativeLeftSide;

    Parser() {};

    Parser(Parser const &);

    void initialize(ParsingTable *parseTable);

    ParseResult parse(TokenWithLexeme token);

    ParseResult panicModeRecovery(TokenWithLexeme token, ERROR error);
};

#endif //COMPILER_PARSER_H