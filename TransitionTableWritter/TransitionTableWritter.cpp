#include <algorithm>
#include "TransitionTableWritter.h"
using namespace std;

void TransitionTableWritter::write(set<Dstate *> dfa, unordered_set<char> inputs, string fileName)
{
    // Open file
    file.open(fileName);

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
    for (auto state : dfa)
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

    // Close file
    file.close();
}

void TransitionTableWritter::fillSpaces(int length)
{
    for (int i = 0; i < length; i++)
        file << " ";
}

void TransitionTableWritter::drawLine(int length)
{
    for (int i = 0; i < length; i++)
        file << "-";
    file << "\n";
}
