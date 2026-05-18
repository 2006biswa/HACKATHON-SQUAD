#include "state.hpp"

namespace core {

    State::State(int num_nodes) {
        //TODO : Initialization
        is_in_team.resize(num_nodes + 1, 0);
        conflict_count.resize(num_nodes + 1, 0);
        current_total_skill = 0;
    }

    void State::add_coder(const Graph& g, int u) {
        // Only add if they are not already in the team
        if (is_in_team[u]) return;
        
        is_in_team[u] = 1;
        current_total_skill += g.skills[u];
        
        // Rapid O(1) cache-friendly loop over neighbors using CSR
        int start = g.offset[u];
        int end = g.offset[u + 1];
        for (int i = start; i < end; ++i) {
            int enemy = g.edges[i];
            conflict_count[enemy]++;
        }
    }

    void State::remove_coder(const Graph& g, int u) {
        if (!is_in_team[u]) return;
        
        is_in_team[u] = 0;
        current_total_skill -= g.skills[u];
        
        int start = g.offset[u];
        int end = g.offset[u + 1];
        for (int i = start; i < end; ++i) {
            int enemy = g.edges[i];
            conflict_count[enemy]--;
        }
    }

    bool State::can_add(int u) const {
        return conflict_count[u] == 0 && is_in_team[u] == 0;
    }

} // namespace core
