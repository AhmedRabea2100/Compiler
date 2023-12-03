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

    State();
    State(bool is_final, Type type, int priority);

    const int get_id() const;

    bool get_is_final() const;
    void set_is_final(bool is_final);

    int get_priority() const;
    void set_priority(int priority);

    Type get_type() const;
    void set_type(Type type);

    vector<Transition> get_transitions();
    void add_transition(Transition t);
    void add_transition(State *to, char input);
    void add_transition(State *to);

private:
    static int max_id;
    const int id;
    bool is_final;
    int priority;
    Type type;
    vector<Transition> transitions;
};
