#include <algorithm>
#include "OutputWritter.h"
using namespace std;

OutputWritter::OutputWritter(std::string fileName){
    file.open(fileName);
}

void OutputWritter::writeTransitionTable(set<Dstate *> minimized_dfa, unordered_set<char> inputs){
    // Sort inputs
    vector<char> sorted_inputs;
    for (auto input : inputs)
        sorted_inputs.push_back(input);
    sort(sorted_inputs.begin(), sorted_inputs.end());

    // Write inputs
    drawLine(sorted_inputs.size() * 4 + 15);
    fillSpaces(15);
    for (auto input : sorted_inputs)
    {
        file << input << '\t';
    }
    file << "\n";
    drawLine(sorted_inputs.size() * 4 + 15);

    // Sort states by id
    vector<Dstate *> sorted_states;
    for (auto state : minimized_dfa)
        sorted_states.push_back(state);
    sort(sorted_states.begin(), sorted_states.end(), [](Dstate *a, Dstate *b)
         { return a->get_id() < b->get_id(); });

    // Write states
    for (int i = 0; i < sorted_states.size() - 1; i++)
    {
        Dstate *state = sorted_states[i + 1];
        file << i;
        string s = to_string(i);
        fillSpaces(5 - s.length());
        if (i == 0)
        {
            file << "Start";
            fillSpaces(5);
        }
        else
        {
            string type = state->get_type();
            file << type;
            fillSpaces(10 - type.length());
        }
        for (auto input : sorted_inputs)
        {
            string id = to_string(state->get_transation(input)->get_id());
            file << id << '\t';
        }
        file << "\n";
    }

    drawLine(sorted_inputs.size() * 4 + 15);
}

void OutputWritter::writeParserResult(ParseResult& parserResult) {
    if (parserResult.msg.empty()) {
        file << parserResult.rule.first.toString() << " ----> " << parserResult.rule.second.stringify() << std::endl;
    } else {
        file << parserResult.msg << std::endl;
    }
}

void OutputWritter::writeParsingTable(ParsingTable* parsingTable) {
    std::map<Symbol, std::map<Symbol, Production>> table = parsingTable->getParsingTable();
    file << "PREDICTIVE PARSING TABLE :" << std::endl;
    drawLine(table.size() * 50);
    //first print terminals
    std::set<Symbol> terminals;
    for (auto &outer_map_pair : table)
        for (auto &inner_map_pair : outer_map_pair.second)
            terminals.insert(inner_map_pair.first);
    fillSpaces(45);
    //print terminals
    for (auto terminal : terminals) {
        file << terminal.name;
        fillSpaces(50 - terminal.name.length());
    }
    file << std::endl;
    drawLine(table.size() * 50);
    //print non-terminals and productions
    for (auto &outer_map_pair : table) {
        file << outer_map_pair.first.name;
        fillSpaces(20 - outer_map_pair.first.name.length());
        file << "|";
        for (auto terminal : terminals) {
            if (!parsingTable->isEmpty(outer_map_pair.first, terminal)) {
                std::string production = table[outer_map_pair.first][terminal].stringify();
                file << production;
                fillSpaces(50 - production.length());
                file << "|";
            } else {
                fillSpaces(50);
                file << "|";
            }
        }
        file << std::endl;
    }

    drawLine(table.size() * 50);
    file << std::endl;
}

void OutputWritter::writeLeftDerivation(std::list<Symbol> *derivationLeftSide,
                                        std::list<Symbol> *stack,
                                        std::string errorMsg) {
    for (auto symbol: *derivationLeftSide) {
        file << symbol.toString() << " ";
    }

    auto endIterator = stack->end(), newLine = stack->end();
    endIterator--, newLine--, newLine--;

    for (auto it = stack->begin(); it != endIterator; it++) {
        file << (*it).toString() << (it == newLine ? "" : " ");
    }

    if (!errorMsg.empty()) {
        file << " ---> " << errorMsg;
    }

    file << std::endl;
}

void OutputWritter::writeMatch(std::string currentMatch, std::string currentMatchType) {
    file << "Token: "  << currentMatch;
    for(int i = 0; i < 25 - currentMatch.size(); i++)
        file << " ";
    file << "Type: " << currentMatchType << std::endl;
}

void OutputWritter::writeError(std::string error) {
    file << error << std::endl;
}

void OutputWritter::closeFile() {
    file.close();
}

void OutputWritter::fillSpaces(int length){
    for (int i = 0; i < length; i++)
        file << " ";
}

void OutputWritter::drawLine(int length)
{
    for (int i = 0; i < length; i++)
        file << "-";
    file << "\n";
}
