
#ifndef COMPILER_DSTATE_H
#define COMPILER_DSTATE_H

#include "../NFA/State.h"

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Dstate
{
public:
    Dstate(set<State*> U);
    Dstate();
    int get_id() const ;
    bool get_is_final() const ;
    string get_type() ;
    int get_priority() const ;
    set<State*> get_U();


    void set_transation(char input, Dstate* to);
    Dstate* get_transation(char input);

    map<char, Dstate*> transitions;


protected:

private:
    const int id;
    bool is_final;
    string type;
    int priority;
    set<State*> U;

    static int id_generator;

};



#endif //COMPILER_DSTATE_H

