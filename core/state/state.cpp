#include "state.hpp"

namespace core {

    State::State(int num_nodes) {
        // TODO: Initialize arrays to empty
    }

    void State::add_coder(const Graph& g, int u) {
        // TODO: Mark coder 'u' as in team.
        // TODO: Loop over 'u's neighbors in the graph and increase their conflict_count.
        // TODO: Add 'u's skill to current_total_skill.
    }

    void State::remove_coder(const Graph& g, int u) {
        // TODO: Mark coder 'u' as out of team.
        // TODO: Loop over 'u's neighbors and decrease their conflict_count.
        // TODO: Subtract 'u's skill from current_total_skill.
    }

    bool State::can_add(int u) const {
        // TODO: Return true if conflict_count[u] == 0 and not already in team
        return false;
    }

} // namespace core
