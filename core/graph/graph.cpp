#include "graph.hpp"

namespace core {

    Graph::Graph(int n, int m) {
        // TODO: Initialize the arrays to hold N coders and M edges
    }

    void Graph::add_edge(int u, int v) {
        // TODO: Store the edge temporarily before building CSR
    }

    void Graph::set_skill(int u, int64_t skill) {
        // TODO: Save the skill rating
    }

    void Graph::build_csr() {
        // TODO: Convert the temporary edges into the blazing fast CSR offset/edges arrays
    }

    std::vector<ConnectedComponent> split_into_components(const Graph& g) {
        // TODO: Run a DFS (Depth First Search) to find disconnected islands of coders
        return {};
    }

} // namespace core
