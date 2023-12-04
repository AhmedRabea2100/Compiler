#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <string>
#include <vector>

enum Type {
    CHAR,
    OPERATOR,
};

struct RegexChar {
    char c;
    Type cType;
};

class Token {
public:
    Token(int priority, std::string type, std::vector<RegexChar *> postfixRegex);
    int getPriority();
    std::string getType();
    std::vector<RegexChar*> getPostfixRegex();
private:
    int priority;
    std::string type;
    std::vector<RegexChar*> postfixRegex;
};

#endif //COMPILER_TOKEN_H