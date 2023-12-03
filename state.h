#pragma once
#include "transition.h"
#include <vector>
using namespace std;

class State
{
public:
    enum class Type
    {
        NONE,
        id,
        number,
        keyword,
        op,
        symbol
    };

    State(bool is_final, Type type, int priority);
    const int get_id() const;
    const bool get_is_final() const;
    const Type get_type() const;
    const int get_priority() const;
    vector<Transition> get_transitions();
    void add_transition(Transition t);
    void add_transition(State* to, char input);
    void add_transition(State* to);

private:
    static int max_id;
    const int id;
    const bool is_final;
    const Type type;
    const int priority;
    vector<Transition> transitions;
};
