#include "Minimize.h"

set<Dstates *> Minimize::minimize(const set<Dstates *> &dfa) {
    vector<set<Dstates *>> partitions(2);
    map<int, int> partition_id;

    for (auto *state: dfa) {
        if (state->get_is_final()) {
            partition_id[state->get_id()] = 0;
            partitions[0].insert(state);
        } else {
            partition_id[state->get_id()] = 1;
            partitions[1].insert(state);
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;
        vector<set<Dstates *>> current_partitions;
        map<int, int> cur_partition_id;

        for (auto &part: partitions) {

            for (auto ptr1 = part.begin(); ptr1 != part.end(); ptr1++) {
                if (cur_partition_id.count((*ptr1)->get_id()) != 0)
                    continue;

                current_partitions.emplace_back();
                current_partitions.back().insert(*ptr1);
                cur_partition_id[(*ptr1)->get_id()] = (int) current_partitions.size() - 1;

                auto cur_transitions = (*ptr1)->transitions;

                for (auto ptr2 = ptr1; ptr2 != part.end(); ptr2++) {
                    if (ptr1 == ptr2)
                        continue;

                    auto nxt_transitions = (*ptr2)->transitions;

                    if (cur_transitions.size() != nxt_transitions.size())
                        continue;

                    bool same = true;
                    for (auto [input, state]: cur_transitions) {
                        auto to1 = nxt_transitions[input]->get_id();
                        auto to2 = state->get_id();

                        if (nxt_transitions.count(input) == 0 ||
                            partition_id.count(to1) != partition_id.count(to2) ||
                            partition_id[to1] != partition_id[to2]) {
                            same = false;
                            changed = true;
                            break;
                        }
                    }

                    if (!same)
                        continue;

                    current_partitions.back().insert(*ptr2);
                    cur_partition_id[(*ptr2)->get_id()] = (int) current_partitions.size() - 1;
                }
            }

        }
        partitions = current_partitions;
        partition_id = cur_partition_id;
    }

    set<Dstates *> minimized_dfa;
    vector<Dstates *> states;
    int id = 0;
    for (auto &part: partitions) {
        states.emplace_back(new Dstates(id ++));
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
                if(partition_id.count(to) != 0)
                    new_state->set_transation(input, states[partition_id[to]]);
            }
        }
        minimized_dfa.insert(new_state);
    }
    return minimized_dfa;
}
