#include <iostream>
#include <vector>
#include "core/graph/graph.hpp"
#include "solver/reductions/reductions.hpp"
#include "solver/memetic/memetic.hpp"
#include "utils/timer/timer.hpp"

// What this does:
// This is the absolute starting point of the CLI application.
int main(int argc, char** argv) {
    // Fast I/O for massive test cases
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    // 1. Start the strict 295-second timer immediately.
    utils::Timer timer(295.0);

    // 2. Read N (coders) and M (conflicts) from std::cin
    // TODO: Parse standard input
    int N, M;
    if (!(std::cin >> N >> M)) return 0;

    core::Graph g(N, M);
    for (int i = 1; i <= N; ++i) {
        int64_t skill;
        std::cin >> skill;
        g.set_skill(i, skill);
    }
    for (int i = 0; i < M; ++i) {
        int u, v;
        std::cin >> u >> v;
        g.add_edge(u, v);
    }
    g.build_csr();

    // 3. Apply exact reductions to shrink the graph mathematically
     // TODO: Call solver::Reductions::apply_reductions
    std::vector<int> forced_in, forced_out;
    solver::Reductions::apply_reductions(g, forced_in, forced_out);

    // 4. Initialize the evolutionary population (GRASP)
    // TODO: Instantiate MemeticAlgorithm and initialize_population
    solver::MemeticAlgorithm memetic(g, 50); // Population size of 50
    memetic.initialize_population(g);

    // 5. Run the endless loop! It will automatically break when timer.is_time_up() == true
     // TODO: Call memetic.run_evolution()
    while (!timer.is_time_up()) {
        memetic.run_evolution(g);
    }

    // 6. Output the absolute best team found
    // TODO: Print the total sum
    // TODO: Print the space-separated sorted indices
    int64_t best_score = -1;
    int best_idx = -1;
    for (int i = 0; i < memetic.population.size(); ++i) {
        if (memetic.population[i].state.current_total_skill > best_score) {
            best_score = memetic.population[i].state.current_total_skill;
            best_idx = i;
        }
    }

    if (best_idx != -1) {
        std::cout << best_score << "\n";
        std::vector<int> team;
        for (int i = 1; i <= N; ++i) {
            if (memetic.population[best_idx].state.is_in_team[i]) {
                team.push_back(i);
            }
        }
        std::cout << team.size() << "\n";
        for (int u : team) std::cout << u << " ";
        std::cout << "\n";
    }

    return 0;
}
