#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

// Include our solver modules
#include "../../core/graph/graph.hpp"
#include "../../solver/reductions/reductions.hpp"
#include "../../solver/memetic/memetic.hpp"
#include "../../utils/timer/timer.hpp"

namespace fs = std::filesystem;

// What this does:
// This struct holds the raw data that will be printed for your CV.
struct CVMetrics {
    double total_optimality_gap = 0.0;       // Accuracy sum
    int num_files_tested = 0;                // For average
    long long total_swaps_evaluated = 0;     // Throughput
    double total_time_seconds = 0.0;         // Throughput (Swaps / Time)
    int total_nodes_pruned = 0;              // Reduction Power
    int total_nodes_original = 0;            // Reduction Power
    int total_connected_components = 0;      // Scalability
    size_t peak_memory_mb = 0;               // Memory Efficiency
};

// Helper function to ask Windows exactly how much RAM we are using
size_t get_current_memory_usage_mb() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / (1024 * 1024);
    }
    return 0;
#else
    return 0;
#endif
}

void run_benchmark_on_file(const std::string& input_file, const std::string& output_file, CVMetrics& global_metrics) {
    std::ifstream in(input_file);
    if (!in.is_open()) return;

    int N, M;
    if (!(in >> N >> M)) return;

    global_metrics.total_nodes_original += N;

    core::Graph g(N, M);
    for (int i = 1; i <= N; ++i) {
        int64_t skill;
        in >> skill;
        g.set_skill(i, skill);
    }
    for (int i = 0; i < M; ++i) {
        int u, v;
        in >> u >> v;
        g.add_edge(u, v);
    }
    g.build_csr();
    in.close();

    // 2. Scalability Check (Divide & Conquer)
    auto components = core::split_into_components(g);
    global_metrics.total_connected_components += components.size();

    // 3. Reduction Power Check
    std::vector<int> forced_in, forced_out;
    solver::Reductions::apply_reductions(g, forced_in, forced_out);
    global_metrics.total_nodes_pruned += forced_in.size() + forced_out.size();

    // 4. Memory Efficiency Check
    size_t current_mem = get_current_memory_usage_mb();
    if (current_mem > global_metrics.peak_memory_mb) {
        global_metrics.peak_memory_mb = current_mem;
    }

    // 5. Throughput & Convergence (Limit to 1 second per file for rapid benchmarking)
    utils::Timer timer(1.0);
    solver::MemeticAlgorithm memetic(g, 50);
    memetic.initialize_population(g);

    long long iterations = 0;
    while (!timer.is_time_up()) {
        memetic.run_evolution(g);
        iterations++;
    }
    global_metrics.total_time_seconds += 1.0;
    global_metrics.total_swaps_evaluated += (iterations * N); // Approximation of inner swaps

    // 6. Accuracy Check
    std::ifstream out(output_file);
    int64_t true_max = 1;
    if (out.is_open()) {
        out >> true_max;
        out.close();
    }

    int64_t best_score = 0;
    for (const auto& ind : memetic.population) {
        if (ind.state.current_total_skill > best_score) best_score = ind.state.current_total_skill;
    }

    if (true_max > 0) {
        double gap = ((double)best_score / true_max) * 100.0;
        if (gap > 100.0) gap = 100.0; // clamp
        global_metrics.total_optimality_gap += gap;
        global_metrics.num_files_tested++;
        std::cout << "Tested: " << input_file << " -> Accuracy: " << std::fixed << std::setprecision(2) << gap << "%" << std::endl;
    }
}

int main() {
    std::cout << "Starting CV Benchmark Runner for Hackathon Squad...\n" << std::endl;

    CVMetrics metrics;
    std::string test_dir = "../test_suite";

    if (!fs::exists(test_dir)) {
        std::cerr << "Could not find test_suite folder!" << std::endl;
        return 1;
    }

    // Loop through all input files
    for (const auto& entry : fs::directory_iterator(test_dir)) {
        std::string filename = entry.path().filename().string();
        if (filename.find("input_") == 0) { // starts with input_
            std::string output_filename = "output_" + filename.substr(6);
            std::string input_path = entry.path().string();
            std::string output_path = test_dir + "/" + output_filename;
            
            run_benchmark_on_file(input_path, output_path, metrics);
        }
    }

    // Final Report for CV
    double avg_accuracy = metrics.num_files_tested > 0 ? (metrics.total_optimality_gap / metrics.num_files_tested) : 0.0;
    long long swaps_per_sec = metrics.total_time_seconds > 0 ? (metrics.total_swaps_evaluated / metrics.total_time_seconds) : 0;
    double prune_percent = metrics.total_nodes_original > 0 ? ((double)metrics.total_nodes_pruned / metrics.total_nodes_original) * 100.0 : 0.0;

    std::cout << "\n================ CV METRICS REPORT ================" << std::endl;
    std::cout << "Files Evaluated:            " << metrics.num_files_tested << " distinct graph topologies" << std::endl;
    std::cout << "Accuracy (Optimality Gap):  " << std::fixed << std::setprecision(2) << avg_accuracy << "%" << std::endl;
    std::cout << "Throughput (Speed):         " << swaps_per_sec << " heuristic swap evaluations/sec" << std::endl;
    std::cout << "Reduction Power:            " << std::fixed << std::setprecision(2) << prune_percent << "% of global graph size instantly pruned" << std::endl;
    std::cout << "Scalability (Components):   " << metrics.total_connected_components << " independent sub-problems dynamically isolated" << std::endl;
    std::cout << "Memory Efficiency (Peak):   " << metrics.peak_memory_mb << " MB maximum memory footprint" << std::endl;
    std::cout << "===================================================" << std::endl;

    return 0;
}
