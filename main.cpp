#include <iostream>
#include <set>
#include <map>
#include "state.h"
#include "transition.h"
#include "DFA.h"
#include "Minimize.h"

int main() {
    State s0 = State(false, {}, 0);
    State s1 = State(true, {}, 0);
    State s2 = State(true, {}, 0);
    State s3 = State(true, {}, 0);
    State s4 = State(false, {}, 0);
    State s5 = State(true, {}, 0);
    State s6 = State(true, {}, 0);

    s0.add_transition(&s1, '0');
    s0.add_transition(&s2, '1');
    s1.add_transition(&s1, '0');
    s1.add_transition(&s3, '1');
    s2.add_transition(&s4, '0');
    s2.add_transition(&s5, '1');
    s3.add_transition(&s4, '0');
    s3.add_transition(&s6, '1');
    s4.add_transition(&s1, '0');
    s4.add_transition(&s2, '1');
    s5.add_transition(&s4, '1');
    s6.add_transition(&s1, '0');
    s6.add_transition(&s1, '1');

    DFA dfa;
    set<Dstates *> dfa_states = dfa.NFA_to_DFA({&s0});
//    for (auto d: dfa_states) {
//        cout << d->get_id() << endl;
//        for (auto t: d->transitions) {
//            cout << t.first << " " << t.second->get_id() << endl;
//        }
//    }

    set<Dstates *> minimized = Minimize::minimize(dfa_states);
    for (auto d: minimized) {
        cout << d->get_id() << endl;
        for (auto t: d->transitions) {
            cout << t.first << " " << t.second->get_id() << endl;
        }
    }

}
