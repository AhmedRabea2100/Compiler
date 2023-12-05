#include "GrammarParser.h"


// Lexical Rules Input File Format
const char REGEX_SYM = ':';
const char REDEF_SYM = '=';
const char RESERVED_START = '{';
const char RESERVED_END = '}';
const char PUNC_START = '[';
const char PUNC_END = ']';
const char LAMBDA_SYM = 'L';

// REGEX OPERATIONS
const char CONCATENATE = '$';
const char OR = '|';
const char STAR = '*';
const char PLUS = '+';
const char OPEN_BRACKET = '(';
const char CLOSE_BRACKET = ')';
const char RANGE = '-';

const int MAX_PRIORITY = 1e9;

// REGEX OPERATIONS PRECEDENCE
std::unordered_map<char, int> precedence = {
        {OPEN_BRACKET, 0},
        {CLOSE_BRACKET, 0},
        {OR, 1},
        {RANGE, 2},
        {CONCATENATE, 3},
        {PLUS, 4},
        {STAR, 5}
};

std::vector<Token *> tokens;
std::unordered_map<std::string, std::string> RegularDefinitions;

GrammarParser::GrammarParser() {}

std::vector<Token *> GrammarParser::getTokens(std::string grammarFilePath) {
    std::ifstream grammarFile(grammarFilePath);
    std::string line;
    int priority = 1e3;
    while (std::getline(grammarFile, line)) {
        line = trim(line, " \t");
        line = removeExtraSpaces(line);
        if(line.empty())
            continue;

        if(checkReserved(line))
            handleReserved(line);
        else if(checkPunctuation(line))
            handlePunctuation(line);
        else if(checkRegularExpression(line))
            handleRegularExpression(line, priority);
        else if(checkRegularDefinition(line))
            handleRegularDefinition(line);
        else
            std::cout << "Invalid line: " << line << std::endl;
        priority--;
    }
    std::cout << "Finished Parsing Grammar File" << std::endl;
    grammarFile.close();
    return tokens;
}

bool GrammarParser::checkRegularExpression(std::string str) {
    size_t spacePos = str.find(' ');
    if (spacePos != std::string::npos) {
        std::string word1 = str.substr(0, spacePos);
        std::string word2;
        size_t secondSpacePos = str.find(' ', spacePos + 1);
        if (secondSpacePos != std::string::npos) {
            word2 = str.substr(spacePos + 1, secondSpacePos - spacePos - 1);
        }
        else {
            word2 = str.substr(spacePos + 1);
        }
        return word1[word1.length() - 1] == REGEX_SYM || word2[0] == REGEX_SYM;
    }
    return false;
}

void GrammarParser::handleRegularExpression(std::string str, int priority) {
    int idx = str.find(REGEX_SYM);
    std::string type = trim(str.substr(0, idx), " \t");
    std::string regex = trim(str.substr(idx + 1), " \t");
    regex = replaceVariableWithRegex(regex);
    regex = replaceSpacesWith(regex, "");
    regex = addConcatenateSymbol(regex);
    std::vector<RegexChar*> postfix = infixToPostfix(regex);
    tokens.push_back(new Token(priority, type, postfix));
}

bool GrammarParser::checkRegularDefinition(std::string str) {
    size_t equalPos = str.find(' ');
    if (equalPos != std::string::npos) {
        std::string word1 = str.substr(0, equalPos);
        std::string word2;
        size_t secondSpacePos = str.find(' ', equalPos + 1);
        if (secondSpacePos != std::string::npos) {
            word2 = str.substr(equalPos + 1, secondSpacePos - equalPos - 1);
        }
        else {
            word2 = str.substr(equalPos + 1);
        }
        return word1[word1.length() - 1] == REDEF_SYM || word2[0] == REDEF_SYM;
    }
    return false;
}

void GrammarParser::handleRegularDefinition(std::string str) {
    int idx = str.find(REDEF_SYM);
    std::string variable = trim(str.substr(0, idx), " \t");
    std::string regex = trim(str.substr(idx + 1), " \t");
    regex = replaceVariableWithRegex(regex);
    RegularDefinitions[variable] = replaceSpacesWith(regex, "");
}

bool GrammarParser::checkPunctuation(std::string str) {
    return str[0] == PUNC_START && str[str.length() - 1] == PUNC_END;
}

void GrammarParser::handlePunctuation(std::string str) {
    str = str.substr(1, str.length() - 2);
    std::string word = "";
    for (char i : str) {
        if(i == '\\')
            continue;
        if (i == ' ') {
            if(!word.empty()){
                std::vector<RegexChar*> v{createRegexChar(word[0], CHAR)};
                tokens.push_back(new Token(MAX_PRIORITY, word ,v));
            }
                word = "";
        }
        else {
            word += i;
        }
    }
    if(!word.empty()){
        std::vector<RegexChar*> v{createRegexChar(word[0], CHAR)};
        tokens.push_back(new Token(MAX_PRIORITY, word ,v));
    }
}

bool GrammarParser::checkReserved(std::string str) {
    return str[0] == RESERVED_START && str[str.length() - 1] == RESERVED_END;
}

void GrammarParser::handleReserved(std::string str) {
    str = str.substr(1, str.length() - 2);
    std::string word = "";
    for (char i : str) {
        if (i == ' ') {
            if(!word.empty()){
                std::vector<RegexChar*> postfix = infixToPostfix( addConcatenateSymbol(word));
                tokens.push_back(new Token(MAX_PRIORITY, word ,postfix));
            }
            word = "";
        }
        else {
            word += i;
        }
    }
    if(!word.empty()) {
        std::vector<RegexChar*> postfix = infixToPostfix( addConcatenateSymbol(word));
        tokens.push_back(new Token(MAX_PRIORITY, word ,postfix));
    }
}

std::vector<RegexChar*> GrammarParser::infixToPostfix(std::string str) {
    std::vector<RegexChar*> postfix;
    std::stack<char> s;
    for(int i = 0; i < str.length(); i++){
        char c = str[i];
        bool op = false;
        if(c == OR || c == CONCATENATE || c == STAR || c == PLUS || c == RANGE)
            op = true;
        else if(c == '\\') {
            i++;
            c = str[i];
        }
        if(c == LAMBDA_SYM) {
            c = ' ';
        }

        if(c == OPEN_BRACKET){
            s.push(c);
        }
        else if(c == CLOSE_BRACKET){
            while(!s.empty() && s.top() != OPEN_BRACKET){
                postfix.push_back(createRegexChar(s.top(), OPERATOR));
                s.pop();
            }
            s.pop();
        }
        else if(op && (c == OR || c == CONCATENATE || c == STAR || c == PLUS || c == RANGE)){
            while(!s.empty() && precedence[s.top()] >= precedence[c]){
                postfix.push_back(createRegexChar(s.top(), OPERATOR));
                s.pop();
            }
            s.push(c);
        }
        else{
            postfix.push_back(createRegexChar(c, CHAR));
        }
    }
    while(!s.empty()){
        postfix.push_back(createRegexChar(s.top(), OPERATOR));
        s.pop();
    }
    return postfix;
}

std::string GrammarParser::addConcatenateSymbol(std::string str) {
    std::string newStr = "";
    for(int i = 0; i < str.length() - 1; i++){
        char c1 = str[i], c2 = str[i + 1];
        newStr += c1;
        if(c1 != OPEN_BRACKET && c1 != OR &&  c1 != RANGE && c1 != '\\')
            if(c2 != CLOSE_BRACKET && c2 != OR && c2 != STAR && c2 != PLUS && c2 != RANGE)
                newStr += CONCATENATE;
    }
    newStr += str[str.length() - 1];
    return newStr;
}

std::string GrammarParser::replaceVariableWithRegex(std::string str) {
    std::string newStr = "";
    for (int i = 0; i < str.length(); i++) {
        std::string word = "";
        while (i < str.length() && isalnum(str[i])) {
            word += str[i];
            i++;
        }
        if(word.empty())
            newStr += str[i];
        else if (RegularDefinitions.find(word) != RegularDefinitions.end()) {
            newStr +=  OPEN_BRACKET + RegularDefinitions[word] + CLOSE_BRACKET;
            i--;
        }
        else {
            newStr += word;
            i--;
        }
    }
    return newStr;
}

struct RegexChar* GrammarParser::createRegexChar(char c, Type charType) {
    struct RegexChar* regexChar = new struct RegexChar;
    regexChar->c = c;
    regexChar->cType = charType;
    return regexChar;
}

std::string GrammarParser::replaceSpacesWith(std::string str, std::string replaceWith) {
    std::string newStr = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ')
            newStr += replaceWith;
        else
            newStr += str[i];
    }
    return newStr;
}

std::string GrammarParser::removeExtraSpaces(std::string str) {
    std::string newStr = "";
    for (int i = 0; i < str.length(); i++) {
        if (i != str.length() - 1 &&  str[i] == ' ' && str[i + 1] == ' ')
            continue;
        newStr += str[i];
    }
    return newStr;
}

std::string GrammarParser::trim(std::string str, std::string whitespace) {
    size_t start = str.find_first_not_of(whitespace);
    size_t end = str.find_last_not_of(whitespace);
    if (start == std::string::npos)
        return "";
    return str.substr(start, end - start + 1);
}