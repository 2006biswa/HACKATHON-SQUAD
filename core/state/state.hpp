#pragma once
#include "graph.hpp"
#include <vector>
#include <cstdint>

namespace core {

    // What this does:
    // This tracks WHO is in the team right now. 
    // It changes millions of times a second, so we track it using flat integers to be extremely fast.
    class State {
    public:
        // is_in_team[i] is true if coder i is currently selected.
        std::vector<uint8_t> is_in_team;
        
        // conflict_count[i] tells us how many enemies of coder i are currently in the team.
        // If this is > 0, we cannot add coder i!
        std::vector<int> conflict_count;
        
        int64_t current_total_skill;

        // Constructor
        State(int num_nodes);

        // Add a coder to the team, and update conflict_count for all their enemies
        void add_coder(const Graph& g, int u);

        // Remove a coder, and update conflict_count for all their enemies
        void remove_coder(const Graph& g, int u);

        // Check if a coder is allowed to be added (i.e. conflict_count == 0)
        bool can_add(int u) const;
    };

} // namespace core
