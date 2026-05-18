#pragma once
#include <vector>
#include <cstdint>

namespace core {

    // What this does: 
    // This is the CSR Graph. Instead of using standard classes, we use flat arrays.
    // This makes the CPU extremely fast because all data is physically next to each other in RAM.
    class Graph {
    public:
        // The number of coders (N) and rivalries (M)
        int num_nodes;
        int num_edges;

        // Skill ratings for each coder
        std::vector<int64_t> skills;

        // CSR (Compressed Sparse Row) arrays:
        // offset[i] tells us where coder i's enemies start in the edges array.
        std::vector<int> offset;
        std::vector<int> edges;

        // Constructor
        Graph(int n, int m);

        // Function to add a rivalry between two coders
        void add_edge(int u, int v);

        // Function to set a coder's skill
        void set_skill(int u, int64_t skill);

        // Finalizes the CSR structure so we can start searching
        void build_csr();
    };

    // Component Splitting
    // We want to break the big graph into smaller, disconnected sub-graphs to solve them easily.
    struct ConnectedComponent {
        std::vector<int> nodes; // The coders in this sub-graph
    };

    std::vector<ConnectedComponent> split_into_components(const Graph& g);

} // namespace core
