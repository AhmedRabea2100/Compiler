#include "CodeParser.h"

std::vector<char> fileChars;
std::ofstream outputFile;
int index = 0;
unordered_map<std::string , std::string> symbolTable;

CodeParser::CodeParser(set<Dstate *> states): minStates(states) {}

std::vector<char> CodeParser::parseFile(std::string filename) {
    std::ifstream file(filename);
    std::string temp;
    char c;
    while (file.get(c)) { // loop getting single characters
        fileChars.push_back(c);
    }
    file.close();
    std::cout << "Finished Parsing Code File" << std::endl;


    // get intial state
    Dstate *currentState;
    for (auto &state: minStates) {
        if (state->get_id() == 0) {
            currentState = state;
            break;
        }
    }

    outputFile.open("output.txt");
    while (index < fileChars.size()) {
        if (!match(currentState, index)) {
            outputFile << "ERROR: " << fileChars[index] << std::endl;
            index++;
        }
    }
    outputFile.close();
    return fileChars;
}

bool CodeParser::match(Dstate *state, int startIndex) {
    Dstate *currentState = state, *nextState = nullptr;
    char c;
    std::string currentMatchType = "";
    std::string currentMatch = "";
    int currentMatchIndex = -1;

    // loop on all chars
    for (int i = startIndex; i < fileChars.size(); i++) {
        c = fileChars[i];
        if (currentState->transitions.find(c) == currentState->transitions.end() || iswspace(c) || currentState->get_id() == -1)
            break;

        nextState = currentState->transitions[c];
        if (nextState->get_is_final()) {
            currentMatchIndex = i;
            currentMatchType = nextState->get_type();
        }
        currentState = nextState;
        index = i;
    }
    if (iswspace(c) && currentMatchIndex == -1) {
        index++;
        return true;
    }
    else if (currentMatchIndex == -1)
        return false;
    else {
        for(int j = startIndex; j <= currentMatchIndex; j++) {
            currentMatch += fileChars[j];
        }
        symbolTable[currentMatch] = currentMatchType;
        outputFile << "Token: "  << currentMatch;
        for(int i = 0; i < 25 - currentMatch.size(); i++) {
            outputFile << " ";
        }
        outputFile << "Type: " << currentMatchType << std::endl;
        index = currentMatchIndex+ 1;
    }
    return true;
}
