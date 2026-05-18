#include "reductions.hpp"

namespace solver {

    void Reductions::apply_reductions(core::Graph& g, std::vector<int>& forced_in, std::vector<int>& forced_out) {
        // TODO: Loop until no more reductions can be made
        // reduce_degree_zero()
        // reduce_degree_one()
        bool changed = true;
        while (changed) {
            changed = false;
            if (reduce_degree_zero(g, forced_in)) changed = true;
            // if (reduce_degree_one(g, forced_in, forced_out)) changed = true;
        }
    }

    bool Reductions::reduce_degree_zero(core::Graph& g, std::vector<int>& forced_in) {
        // TODO: Find coders with 0 active conflicts.
        // TODO: Add them to 'forced_in' array.
        // TODO: Return true if we actually reduced something.
        bool reduced = false;
        // In a real cascading reduction, we would track dynamic degrees.
        // For now, we do a static pass over the initial graph degrees.
        for (int i = 1; i <= g.num_nodes; ++i) {
            int degree = g.offset[i + 1] - g.offset[i];
            if (degree == 0) {
                // If it's not already in forced_in, we'd add it here.
                // We will refine this logic to track "active" status later.
            }
        }
        return reduced;
    }

    bool Reductions::reduce_degree_one(core::Graph& g, std::vector<int>& forced_in, std::vector<int>& forced_out) {
        // TODO: Find a coder with exactly 1 enemy.
        // TODO: Use math to decide if we keep the coder or the enemy.
        return false;
    }

} // namespace solver
