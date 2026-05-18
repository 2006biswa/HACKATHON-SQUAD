#include "graph.hpp"

namespace core {

    // TODO: Initialize the arrays to hold N coders and M edges
    Graph::Graph(int n, int m) : num_nodes(n), num_edges(m) {
        skills.resize(n + 1, 0); // 1-based indexing for coders
        temp_adj.resize(n + 1);
    }

    void Graph::add_edge(int u, int v) {
        // TODO: Add the edge to the temporary adjacency list
        temp_adj[u].push_back(v);
        temp_adj[v].push_back(u);
    }

    void Graph::set_skill(int u, int64_t skill) {
        // TODO: Save the skill rating
        skills[u] = skill;
    }

    void Graph::build_csr() {
        //TO DO: Convert the graph into csr flat 1-d array 
        offset.resize(num_nodes + 2, 0);  //one extra to add the endpoint

        //so i created here the offset and  edges vectors
        int current_offset = 0;
        for (int i = 1; i <= num_nodes; ++i) {
            offset[i] = current_offset;
            for (int neighbor : temp_adj[i]) {
                edges.push_back(neighbor); 
                current_offset++;
            }
        }
        offset[num_nodes + 1] = current_offset; // the end boundary
        
        // Free the temporary adjacency list to save memory
        temp_adj.clear();
        temp_adj.shrink_to_fit();
    }


} // namespace core
