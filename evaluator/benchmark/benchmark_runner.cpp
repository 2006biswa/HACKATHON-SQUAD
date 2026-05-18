#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

// Include our solver modules
#include "../../core/graph/graph.hpp"
#include "../../solver/reductions/reductions.hpp"
#include "../../solver/memetic/memetic.hpp"
#include "../../utils/timer/timer.hpp"

// What this does:
// This struct holds the raw data that will be printed for your CV.
struct CVMetrics {
    double average_optimality_gap = 0.0;     // Accuracy
    long long total_swaps_evaluated = 0;     // Throughput
    double total_time_seconds = 0.0;         // Throughput (Swaps / Time)
    int total_nodes_pruned = 0;              // Reduction Power
    int total_nodes_original = 0;            // Reduction Power
    int total_connected_components = 0;      // Scalability
    size_t peak_memory_mb = 0;               // Memory Efficiency
    double average_convergence_time = 0.0;   // Convergence Speed
};

// Helper function to ask Windows exactly how much RAM we are using
size_t get_current_memory_usage_mb() {
#ifdef _WIN32
    // TODO: Use Windows GetProcessMemoryInfo to read PROCESS_MEMORY_COUNTERS
    return 0;
#else
    return 0;
#endif
}

void run_benchmark_on_file(const std::string& input_file, const std::string& output_file, CVMetrics& global_metrics) {
    // 1. Parsing
    // TODO: Read input_file and initialize core::Graph
    
    // 2. Scalability Check (Divide & Conquer)
    // TODO: components = core::split_into_components(graph)
    // TODO: global_metrics.total_connected_components += components.size()

    // 3. Reduction Power Check
    // TODO: Run solver::Reductions::apply_reductions()
    // TODO: global_metrics.total_nodes_pruned += nodes_removed

    // 4. Memory Efficiency Check
    // TODO: size_t current_mem = get_current_memory_usage_mb();
    // TODO: global_metrics.peak_memory_mb = std::max(global_metrics.peak_memory_mb, current_mem);

    // 5. Throughput & Convergence
    // TODO: Run solver::MemeticAlgorithm
    // TODO: global_metrics.total_swaps_evaluated += memetic_algo.get_swap_count()
    // TODO: global_metrics.average_convergence_time += memetic_algo.get_time_to_best_solution()

    // 6. Accuracy Check
    // TODO: Read the true max score from 'output_file'
    // TODO: double gap = (Our_Score / True_Max) * 100.0;
    // TODO: global_metrics.average_optimality_gap += gap;
}

int main() {
    std::cout << "Starting CV Benchmark Runner for Hackathon Squad...\n" << std::endl;

    CVMetrics metrics;

    // TODO: Loop through all 46 test cases in the 'test_suite' folder
    // std::vector<std::string> test_files = get_all_test_files();
    // for (const auto& file : test_files) {
    //      run_benchmark_on_file(file.input, file.output, metrics);
    // }

    // Final Report for CV
    std::cout << "================ CV METRICS REPORT ================" << std::endl;
    std::cout << "Accuracy (Optimality Gap):  " << /* TODO: Calculate average */ "%" << std::endl;
    std::cout << "Throughput (Speed):         " << /* TODO: Calculate Swaps / Second */ " swaps/sec" << std::endl;
    std::cout << "Reduction Power:            " << /* TODO: Pruned / Original * 100 */ "% of graph pruned" << std::endl;
    std::cout << "Scalability (Components):   " << /* TODO: Print total components */ " independent sub-problems found" << std::endl;
    std::cout << "Memory Efficiency (Peak):   " << /* TODO: Print peak_memory_mb */ " MB" << std::endl;
    std::cout << "Convergence Speed:          " << /* TODO: Print average convergence */ " seconds avg" << std::endl;
    std::cout << "===================================================" << std::endl;

    return 0;
}
