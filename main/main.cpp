// Priority Queue Comparative Analysis - Main Driver
// CS 470 Project 1: Dijkstra & Prim with Fibonacci, Pairing, and Binary Heaps
//
// Runs experiments across multiple graph types and sizes, collecting metrics on:
//   - Total runtime
//   - Time spent in extract-min and decrease-key
//   - Number of heap operations
//   - Result values (for correctness verification)

#include "include/benchmark.h"
#include "include/graph_generator.h"
#include <iostream>
#include <cmath>

void run_experiment(const Graph& g, const std::string& graph_type) {
    std::cout << "\n=== " << graph_type
              << " (V=" << g.size()
              << ", E=" << g.edge_count() << ") ===\n\n";

    Benchmark::print_header();

    // Dijkstra with all three heaps
    Benchmark::print_result(Benchmark::run_dijkstra<BinaryHeap>(g, graph_type));
    Benchmark::print_result(Benchmark::run_dijkstra<FibonacciHeap>(g, graph_type));
    Benchmark::print_result(Benchmark::run_dijkstra<PairingHeap>(g, graph_type));

    // Prim with all three heaps
    Benchmark::print_result(Benchmark::run_prim<BinaryHeap>(g, graph_type));
    Benchmark::print_result(Benchmark::run_prim<FibonacciHeap>(g, graph_type));
    Benchmark::print_result(Benchmark::run_prim<PairingHeap>(g, graph_type));
}

int main() {
    std::cout << "============================================================\n";
    std::cout << "  CS 470 - Priority Queue Comparative Analysis\n";
    std::cout << "  Dijkstra & Prim with Fibonacci / Pairing / Binary Heaps\n";
    std::cout << "============================================================\n";

    // Graph sizes to test
    std::vector<int> sizes = {100, 500, 1000, 5000};

    for (int n : sizes) {
        std::cout << "\n\n############################################################\n";
        std::cout << "  GRAPH SIZE: " << n << " vertices\n";
        std::cout << "############################################################\n";

        // 1. Sparse random graph (~4 edges per vertex)
        {
            auto g = GraphGenerator::sparse_random(n);
            run_experiment(g, "Sparse");
        }

        // 2. Dense random graph (~50% edge probability)
        if (n <= 3000) {
            auto g = GraphGenerator::dense_random(n);
            run_experiment(g, "Dense");
        }

        // 3. Grid graph (sqrt(n) x sqrt(n))
        {
            int side = static_cast<int>(std::sqrt(static_cast<double>(n)));
            auto g = GraphGenerator::grid_graph(side, side);
            run_experiment(g, "Grid");
        }

        // 4. Worst-case graph (forces many decrease-key operations)
        {
            auto g = GraphGenerator::worst_case_graph(n);
            run_experiment(g, "WorstCase");
        }
    }

    // Final consolidated output
    std::cout << "\n\n============================================================\n";
    std::cout << "  CONSOLIDATED RESULTS\n";
    std::cout << "============================================================\n\n";
    Benchmark::print_all_results();

    // Verify all heaps produce identical results
    Benchmark::verify_results();

    // Summary
    Benchmark::print_summary();

    return 0;
}
