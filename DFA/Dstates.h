
#ifndef COMPILER_DSTATES_H
#define COMPILER_DSTATES_H

#include "../NFA/State.h"

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Dstates
{
public:
    Dstates(set<State*> U);
    Dstates();
    int get_id() ;
    bool get_is_final() ;
    string get_type() ;
    int get_priority() ;
    set<State*> get_U();


    void set_transation(char input, Dstates* to);
    Dstates* get_transation(char input);

    map<char, Dstates*> transitions;


protected:

private:
    const int id;
    bool is_final;
    string type;
    int priority;
    set<State*> U;

    static int id_generator;

};



#endif //COMPILER_DSTATES_H

