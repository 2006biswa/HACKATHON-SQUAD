#include "memetic.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>


// Remember as i modified this file and added weight to degree initialisation ,2-1 swaps and also plateau search so our computation work increases
// so throughput that is the speed decrease it drops from 117 M swaps /sec to 85 M swaps /sec
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
        std::vector<int> all_nodes(g.num_nodes);
        std::iota(all_nodes.begin(), all_nodes.end(), 1);

        // Precompute base ROI for each node: Weight / (Degree + 1)
        std::vector<double> base_roi(g.num_nodes + 1);
        for (int u = 1; u <= g.num_nodes; ++u) {
            double degree = g.offset[u + 1] - g.offset[u];
            base_roi[u] = g.skills[u] / (degree + 1.0);
        }

        for (int i = 0; i < population.size(); ++i) {
            std::vector<double> noisy_roi = base_roi;
            std::uniform_real_distribution<double> noise_dist(0.5, 1.5); // +/- 50% noise
            
            for (int u = 1; u <= g.num_nodes; ++u) {
                noisy_roi[u] *= noise_dist(rng);
            }

            std::vector<int> candidates = all_nodes;
            std::sort(candidates.begin(), candidates.end(), [&noisy_roi](int a, int b) {
                return noisy_roi[a] > noisy_roi[b];
            });
            
            // Greedily add coders to the team if they have no conflicts
            for (int u : candidates) {
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
        bool active = true;
        int plateau_steps = 0;
        const int MAX_PLATEAU = 20; // Limit sideways moves to prevent infinite loops
        std::uniform_real_distribution<double> plateau_prob(0.0, 1.0);

        while (active) {
            active = false;
            bool strict_improvement = false;
            
            // Scan all coders to see if we can perform a 1-opt or 2-opt swap
            for (int u = 1; u <= g.num_nodes; ++u) {
                if (ind.state.is_in_team[u]) continue; // Already in team
                
                int conflicts = ind.state.conflict_count[u];
                
                // 0-opt: Just add it if it's free! (Makes the set maximal)
                if (conflicts == 0) {
                    ind.state.add_coder(g, u);
                    strict_improvement = true;
                    active = true;
                }
                // 1-opt (1-for-1) and 1-for-2 swaps: Coder 'u' has exactly 1 enemy in the team.
                else if (conflicts == 1) {
                    int enemy_in_team = -1;
                    int start = g.offset[u], end = g.offset[u + 1];
                    for (int j = start; j < end; ++j) {
                        int v = g.edges[j];
                        if (ind.state.is_in_team[v]) {
                            enemy_in_team = v;
                            break;
                        }
                    }
                    
                    if (enemy_in_team != -1) {
                        int64_t delta = g.skills[u] - g.skills[enemy_in_team];
                        bool found_1_for_2 = false;

                        if (delta > 0) {
                            // Standard 1-for-1 strict improvement
                            ind.state.remove_coder(g, enemy_in_team);
                            ind.state.add_coder(g, u);
                            strict_improvement = true;
                            active = true;
                        } else {
                            // Try a 1-for-2 swap: Can we remove enemy_in_team and add BOTH 'u' and another node 'w'?
                            int e_start = g.offset[enemy_in_team], e_end = g.offset[enemy_in_team + 1];
                            for (int k = e_start; k < e_end; ++k) {
                                int w = g.edges[k];
                                if (w != u && !ind.state.is_in_team[w] && ind.state.conflict_count[w] == 1) {
                                    // 'w' also only conflicts with 'enemy_in_team'
                                    int64_t pair_delta = (g.skills[u] + g.skills[w]) - g.skills[enemy_in_team];
                                    if (pair_delta > 0) {
                                        // Ensure 'u' and 'w' don't conflict with each other
                                        auto u_start_it = g.edges.begin() + g.offset[u];
                                        auto u_end_it = g.edges.begin() + g.offset[u + 1];
                                        if (!std::binary_search(u_start_it, u_end_it, w)) {
                                            // No edge between u and w!
                                            ind.state.remove_coder(g, enemy_in_team);
                                            ind.state.add_coder(g, u);
                                            ind.state.add_coder(g, w);
                                            strict_improvement = true;
                                            active = true;
                                            found_1_for_2 = true;
                                            break;
                                        }
                                    }
                                }
                            }

                            // If no 1-for-2 swap was found, try a 1-for-1 plateau swap
                            if (!found_1_for_2 && delta == 0 && !strict_improvement && plateau_steps < MAX_PLATEAU && plateau_prob(rng) < 0.1) {
                                ind.state.remove_coder(g, enemy_in_team);
                                ind.state.add_coder(g, u);
                                plateau_steps++;
                                active = true;
                            }
                        }
                    }
                }
                // 2-opt (2-for-1 swap): Coder 'u' has exactly 2 enemies in the team.
                else if (conflicts == 2) {
                    int e1 = -1, e2 = -1;
                    int start = g.offset[u], end = g.offset[u + 1];
                    for (int j = start; j < end; ++j) {
                        int v = g.edges[j];
                        if (ind.state.is_in_team[v]) {
                            if (e1 == -1) e1 = v;
                            else if (e2 == -1) { e2 = v; break; }
                        }
                    }
                    
                    if (e1 != -1 && e2 != -1) {
                        int64_t delta = g.skills[u] - (g.skills[e1] + g.skills[e2]);
                        if (delta > 0) {
                            ind.state.remove_coder(g, e1);
                            ind.state.remove_coder(g, e2);
                            ind.state.add_coder(g, u);
                            strict_improvement = true;
                            active = true;
                        } else if (delta == 0 && !strict_improvement && plateau_steps < MAX_PLATEAU && plateau_prob(rng) < 0.1) {
                            // Plateau search: occasionally accept a sideways move
                            ind.state.remove_coder(g, e1);
                            ind.state.remove_coder(g, e2);
                            ind.state.add_coder(g, u);
                            plateau_steps++;
                            active = true;
                        }
                    }
                }
            }
        }
    }

} // namespace solver
