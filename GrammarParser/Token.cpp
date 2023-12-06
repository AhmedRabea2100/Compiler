#include "Token.h"

Token::Token(int priority, std::string type, std::vector<RegexChar *> postfixRegex)
        : priority((int)priority), type(type), postfixRegex(postfixRegex) {}

int Token::getPriority() {
    return priority;
}

std::string Token::getType() {
    return type;
}

std::vector<RegexChar *> Token::getPostfixRegex() {
    return postfixRegex;
}
