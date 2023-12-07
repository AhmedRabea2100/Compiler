#include "CodeParser.h"

std::vector<char> fileChars;
std::ofstream outputFile;
int index = 0;

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
        cout<<"rabea"<< endl;
        if (!match(currentState, index)) {
            outputFile << "ERROR" << std::endl;
            index++;
        }
    }
    outputFile.close();
    return fileChars;
}

bool CodeParser::match(Dstate *state, int khara) {
    Dstate *currentState = state, *nextState = nullptr;
    char c;
    std::string currentMatchType = "";
    int currentMatchIndex = -1;


    // loop on all chars
    for (int i = khara; i < fileChars.size(); i++) {
        c = fileChars[i];
        if (currentState->transitions.find(c) == currentState->transitions.end() || iswspace(c) || currentState->get_id() == -1)
            break;

        nextState = currentState->transitions[c];
        if (nextState->get_is_final()) {
            currentMatchIndex = i;
            currentMatchType = nextState->get_type(); // need to be fixed by changing type enum
            cout << "Asdasdas" << nextState->get_type() << endl;
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
        outputFile << currentMatchType << std::endl;
        index = currentMatchIndex+ 1;
    }
    return true;
}
