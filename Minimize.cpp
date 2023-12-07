#include <iostream>
#include "Minimize.h"

set<Dstates *> Minimize::minimize(set<Dstates *> &dfa) {
    vector<set<Dstates *>> partitions(1);
    map<int, int> partition_id;
    map<string, int> fin_mapping;

    dfa.insert(Minimize::prepare_phai(*dfa.begin()));

    for (auto *state: dfa) {
        if (!(state->get_is_final())) {
            partition_id[state->get_id()] = 0;
            partitions[0].insert(state);
        } else {
            if (fin_mapping.count(state->get_type()) != 0) {
                partitions[fin_mapping[state->get_type()]].insert(state);
                partition_id[state->get_id()] = fin_mapping[state->get_type()];
            } else {
                partitions.emplace_back();
                partitions.back().insert(state);
                partition_id[state->get_id()] = (int) partitions.size() - 1;
                fin_mapping[state->get_type()] = (int) partitions.size() - 1;
            }
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;
        vector<set<Dstates *>> cur_partitions;
        map<int, int> cur_partition_id;

        for (auto &part: partitions) {

            for (auto ptr1 = part.begin(); ptr1 != part.end(); ptr1++) {
                if (cur_partition_id.count((*ptr1)->get_id()) != 0)
                    continue;

                cur_partitions.emplace_back();
                cur_partitions.back().insert(*ptr1);
                cur_partition_id[(*ptr1)->get_id()] = (int) cur_partitions.size() - 1;

                auto &cur_transitions = (*ptr1)->transitions;

                for (auto ptr2 = ptr1; ptr2 != part.end(); ptr2++) {
                    if (ptr1 == ptr2 || cur_partition_id.count((*ptr2)->get_id()) != 0)
                        continue;

                    auto &nxt_transitions = (*ptr2)->transitions;

                    bool same = true;
                    for (auto &item: cur_transitions) {
                        int to1 = nxt_transitions[item.first]->get_id();
                        int to2 = item.second->get_id();

                        if (partition_id[to1] != partition_id[to2]) {
                            same = false;
                            changed = true;
                            break;
                        }
                    }

                    if (!same)
                        continue;

                    cur_partitions.back().insert((*ptr2));
                    cur_partition_id[(*ptr2)->get_id()] = (int) cur_partitions.size() - 1;
                }
            }

        }
        partitions = cur_partitions;
        partition_id = cur_partition_id;
    }

    set<Dstates *> minimized_dfa;
    vector<Dstates *> states;
    int id = 0;
    for (auto &part: partitions) {
        states.emplace_back(new Dstates(id++));
        states.back()->set_priority(-1);
    }

    for (int i = 0; i < partitions.size(); i++) {
        auto new_state = states[i];
        for (auto *state: partitions[i]) {
            if (state->get_is_final())
                new_state->set_final(true);
            if (state->get_priority() > new_state->get_priority()) {
                new_state->set_priority(state->get_priority());
                new_state->set_type(state->get_type());
            }

            for (auto [input, trans]: state->transitions) {
                int to = trans->get_id();
                if (partition_id.count(to) != 0)
                    new_state->set_transation(input, states[partition_id[to]]);
            }
        }
        minimized_dfa.insert(new_state);
    }
    return minimized_dfa;
}


Dstates *Minimize::prepare_phai(Dstates *state) {
    Dstates *phai = new Dstates(-1);
    phai->set_type("");
    phai->set_final(false);
    for (auto [input, _]: state->transitions) {
        phai->set_transation(input, phai);
    }
    return phai;
}
