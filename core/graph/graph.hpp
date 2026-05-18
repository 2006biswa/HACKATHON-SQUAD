#pragma once
#include <vector>
#include <cstdint>

namespace core {
   // our first ZERO-ALLOCATion data structure
    // What this does:  
    // This is the CSR Graph. Instead of using the adjacency list that is vector<vector<int>>, we use 2-flat arrays one that is the edges (stores rivals) and the other that is pointers.
    // so  saves our time as cpu does not have to wait for data while cache misses as we always cache hit and use the L1 Cache memory embedded inside the cpu-chip
    // we love continuous memory allocation as it helps in achieving speed
    class Graph {
    public:
        // The number of coders (N) and rivalries (M)
        int num_nodes;
        int num_edges;

        // Skill ratings for each coder
        std::vector<int64_t> skills;

        // CSR (Compressed Sparse Row) arrays:
        // offset[i] tells us where coder i's enemies start in the edges array.  //so latsly we will add an element= size of the edges to the offset marking the end
        std::vector<int> offset;
        std::vector<int> edges;

    private:
        // Temporary staging area when we take the input we don;t get in order the rivalries of 5 so we don;t have any idea how many more enemies of coder 5
        std::vector<std::vector<int>> temp_adj;

    public:
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
    // Divide and conquer rule in order to be faster we are shrinking the problem and being efficient that is being faster
    struct ConnectedComponent {
        std::vector<int> nodes; // The coders in this sub-graph
    };

    std::vector<ConnectedComponent> split_into_components(const Graph& g);

} // namespace core
