#ifndef COMPILER_DSTATES_H
#define COMPILER_DSTATES_H

#include "../NFA/State.h"
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Dstate
{
public:
    Dstate(set<State *> U);
    Dstate();
    Dstate(int id);
    int get_id();
    bool get_is_final();
    string get_type();
    int get_priority();
    set<State *> get_U();

    void set_final(bool is_final);
    void set_priority(int p);
    void set_type(string type);
    void set_transation(char input, Dstate *to);
    Dstate *get_transation(char input);

    map<char, Dstate *> transitions;

private:
    const int id;
    bool is_final;
    string type;
    int priority;
    set<State *> U;

    static int id_generator;
};

#endif // COMPILER_DSTATES_H
