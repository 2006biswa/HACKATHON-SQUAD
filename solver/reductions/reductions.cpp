#include "reductions.hpp"

namespace solver {

    void Reductions::apply_reductions(core::Graph& g, std::vector<int>& forced_in, std::vector<int>& forced_out) {
        // TODO: Loop until no more reductions can be made
        // reduce_degree_zero()
        // reduce_degree_one()
    }

    bool Reductions::reduce_degree_zero(core::Graph& g, std::vector<int>& forced_in) {
        // TODO: Find coders with 0 active conflicts.
        // TODO: Add them to 'forced_in' array.
        // TODO: Return true if we actually reduced something.
        return false;
    }

    bool Reductions::reduce_degree_one(core::Graph& g, std::vector<int>& forced_in, std::vector<int>& forced_out) {
        // TODO: Find a coder with exactly 1 enemy.
        // TODO: Use math to decide if we keep the coder or the enemy.
        return false;
    }

} // namespace solver
