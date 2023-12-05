
#ifndef COMPILER_DSTATES_H
#define COMPILER_DSTATES_H



#include"State.h"

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
     Dstates(int id);
     int get_id() ;
     bool get_is_final() ;
     State::Type get_type() ;
     int get_priority() ;
    set<State*> get_U();


    void set_final(bool is_final);
    void set_priority(int p) ;
    void set_type(State::Type type) ;
    void set_transation(char input, Dstates* to);
    Dstates* get_transation(char input);

    map<char, Dstates*> transitions;


protected:

private:
    const int id;
     bool is_final;
     State::Type type;
     int priority;
    set<State*> U;

    static int id_generator;

};

#endif //COMPILER_DSTATES_H