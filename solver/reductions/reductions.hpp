#pragma once
#include "graph.hpp"

namespace solver {

    // What this does:
    // Kernelization deletes coders from the graph when we can mathematically prove they
    // MUST be in the team, or MUST NOT be in the team.
    // This makes the graph much smaller before we start the Evolutionary loop.
    class Reductions {
    public:
        // Execute all reduction rules
        static void apply_reductions(core::Graph& g, std::vector<int>& forced_in, std::vector<int>& forced_out);

    private:
        // Rule 1: If a coder has 0 enemies, they are guaranteed to be in the team!
        static bool reduce_degree_zero(core::Graph& g, std::vector<int>& forced_in);

        // Rule 2: If coder A has only 1 enemy (coder B), and A's skill is way higher...
        static bool reduce_degree_one(core::Graph& g, std::vector<int>& forced_in, std::vector<int>& forced_out);
    };

} // namespace solver
