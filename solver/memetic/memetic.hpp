#pragma once
#include "../../core/graph/graph.hpp"
#include "../../core/state/state.hpp"
#include <vector>
#include <random>

namespace solver {

    // What this does:
    // This is the core Evolutionary Loop. It generates starting teams, breeds them,
    // mutates them with fast local search, and keeps track of the absolute best team.
    class MemeticAlgorithm {
    public:
        // A struct to represent one valid team in our population
        struct Individual {
            core::State state;
        };

        // The current population of teams
        std::vector<Individual> population;
        //Randomized Greedy Construction (which is Phase 1 of a GRASP algorithm). 
    private:
        // High-speed random number generator
        std::mt19937 rng;

    public:
        // Constructor
        MemeticAlgorithm(const core::Graph& g, int population_size);

        // Phase 2: Generate the initial diverse population using GRASP
        void initialize_population(const core::Graph& g);

        // Phase 3: The main loop (Crossover -> Local Search -> Replace)
        void run_evolution(const core::Graph& g);

    private:
        // Combine two parent teams to make a child team
        Individual crossover(const core::Graph& g, const Individual& parent1, const Individual& parent2);

        // Intensely optimize the child team by swapping coders
        void local_search(const core::Graph& g, Individual& ind);
    };

} // namespace solver
