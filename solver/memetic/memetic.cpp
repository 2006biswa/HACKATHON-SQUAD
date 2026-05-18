#include "memetic.hpp"

namespace solver {

    MemeticAlgorithm::MemeticAlgorithm(const core::Graph& g, int population_size) {
        // TODO: Allocate memory for the population
    }

    void MemeticAlgorithm::initialize_population(const core::Graph& g) {
        // TODO: Loop 'population_size' times.
        // TODO: For each time, greedily build a valid team but randomly pick from top 3 candidates.
        // TODO: Save the team into the population array.
    }

    void MemeticAlgorithm::run_evolution(const core::Graph& g) {
        // TODO: Loop infinitely until the timer strictly cuts us off!
        // TODO: Select 2 parents.
        // TODO: child = crossover(parents).
        // TODO: local_search(child).
        // TODO: If child > worst_in_population, replace worst.
    }

    MemeticAlgorithm::Individual MemeticAlgorithm::crossover(const core::Graph& g, const Individual& p1, const Individual& p2) {
        // TODO: Keep coders present in both p1 and p2.
        // TODO: Fill remaining valid spots greedily.
        return Individual{core::State(g.num_nodes)};
    }

    void MemeticAlgorithm::local_search(const core::Graph& g, Individual& ind) {
        // TODO: Rapidly scan the team for 1-opt and 2-opt swaps.
        // TODO: E.g., If we remove coder A, can we safely add coders B and C?
    }

} // namespace solver
