#include "memetic.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>

namespace solver {

    MemeticAlgorithm::MemeticAlgorithm(const core::Graph& g, int population_size) {
        // TODO: Allocate memory for the population
        // Initialize random generator
        rng.seed(1337); // Seed can be time-based later

        // Allocate memory for the population
        population.reserve(population_size);
        for (int i = 0; i < population_size; ++i) {
            population.push_back(Individual{core::State(g.num_nodes)});
        }
    }

    void MemeticAlgorithm::initialize_population(const core::Graph& g) {
        // TODO: Loop 'population_size' times.
        // TODO: For each time, greedily build a valid team but randomly pick from top 3 candidates.
        // TODO: Save the team into the population array.
        // Create an array of all coder IDs [1, 2, ..., N]
        std::vector<int> all_nodes(g.num_nodes);
        std::iota(all_nodes.begin(), all_nodes.end(), 1);

        for (int i = 0; i < population.size(); ++i) {
            // Randomly shuffle the order we look at coders
            std::shuffle(all_nodes.begin(), all_nodes.end(), rng);
            
            // Greedily add coders to the team if they have no conflicts
            for (int u : all_nodes) {
                if (population[i].state.can_add(u)) {
                    population[i].state.add_coder(g, u);
                }
            }
            // Run a quick local search to guarantee it's a "local maximum"
            local_search(g, population[i]);
        }
    }

    void MemeticAlgorithm::run_evolution(const core::Graph& g) {
        // TODO: Loop infinitely until the timer strictly cuts us off!
        // TODO: Select 2 parents.
        // TODO: child = crossover(parents).
        // TODO: local_search(child).
        // TODO: If child > worst_in_population, replace worst.
        std::uniform_int_distribution<int> dist(0, population.size() - 1);
        
        // Tournament selection: pick 2 random, take the best.
        int p1 = dist(rng);
        int p2 = dist(rng);
        Individual& parent1 = population[p1].state.current_total_skill > population[p2].state.current_total_skill ? population[p1] : population[p2];

        int p3 = dist(rng);
        int p4 = dist(rng);
        Individual& parent2 = population[p3].state.current_total_skill > population[p4].state.current_total_skill ? population[p3] : population[p4];

        // Breed and optimize
        Individual child = crossover(g, parent1, parent2);
        local_search(g, child);

        // Find the weakest link in our population
        int worst_idx = 0;
        int64_t worst_score = population[0].state.current_total_skill;
        for (int i = 1; i < population.size(); ++i) {
            if (population[i].state.current_total_skill < worst_score) {
                worst_score = population[i].state.current_total_skill;
                worst_idx = i;
            }
        }

        // Survival of the fittest!
        if (child.state.current_total_skill > worst_score) {
            population[worst_idx] = child;
        }
    }

    MemeticAlgorithm::Individual MemeticAlgorithm::crossover(const core::Graph& g, const Individual& p1, const Individual& p2) {
        Individual child{core::State(g.num_nodes)};
        
        // 1. Intersection: If both parents agreed to put coder 'u' in the team, they are probably good!
        for (int i = 1; i <= g.num_nodes; ++i) {
            if (p1.state.is_in_team[i] && p2.state.is_in_team[i]) {
                child.state.add_coder(g, i);
            }
        }
        
        // 2. Union: Randomly try to add coders that were in at least one of the parents
        std::vector<int> candidates;
        for (int i = 1; i <= g.num_nodes; ++i) {
            if (p1.state.is_in_team[i] || p2.state.is_in_team[i]) {
                candidates.push_back(i);
            }
        }
        std::shuffle(candidates.begin(), candidates.end(), rng);
        for (int u : candidates) {
            if (child.state.can_add(u)) {
                child.state.add_coder(g, u);
            }
        }
        
        return child;
    }

    void MemeticAlgorithm::local_search(const core::Graph& g, Individual& ind) {
        // TODO: Rapidly scan the team for 1-opt and 2-opt swaps.
        // TODO: E.g., If we remove coder A, can we safely add coders B and C?
        bool improved = true;
        while (improved) {
            improved = false;
            
            // Scan all coders to see if we can perform a 1-opt or 2-opt swap
            for (int u = 1; u <= g.num_nodes; ++u) {
                if (ind.state.is_in_team[u]) continue; // Already in team
                
                int conflicts = ind.state.conflict_count[u];
                
                // 0-opt: Just add it if it's free! (Makes the set maximal)
                if (conflicts == 0) {
                    ind.state.add_coder(g, u);
                    improved = true;
                }
                // 1-opt: Coder 'u' has exactly 1 enemy in the team.
                else if (conflicts == 1) {
                    int enemy_in_team = -1;
                    int start = g.offset[u];
                    int end = g.offset[u + 1];
                    for (int j = start; j < end; ++j) {
                        int v = g.edges[j];
                        if (ind.state.is_in_team[v]) {
                            enemy_in_team = v;
                            break;
                        }
                    }
                    
                    // If coder 'u' is better than the enemy, swap them!
                    if (enemy_in_team != -1 && g.skills[u] > g.skills[enemy_in_team]) {
                        ind.state.remove_coder(g, enemy_in_team);
                        ind.state.add_coder(g, u);
                        improved = true;
                    }
                }
            }
        }
    }

} // namespace solver
