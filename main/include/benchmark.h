#pragma once
// Benchmarking framework for comparative analysis of priority queues
// Collects: total runtime, time in extract-min and decrease-key,
//           number of heap operations, and result values for verification

#include "graph.h"
#include "fibonacci_heap.h"
#include "pairing_heap.h"
#include "binary_heap.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <limits>
#include <cmath>

struct BenchmarkResult {
    std::string algorithm;
    std::string heap_type;
    std::string graph_type;
    int num_vertices;
    int num_edges;
    double total_time_ms;
    int insert_ops;
    int extract_min_ops;
    int decrease_key_ops;
    double extract_min_time_ms;
    double decrease_key_time_ms;
    double result_value; // sum of shortest distances (Dijkstra) or MST weight (Prim)
};

class Benchmark {
public:
    static std::vector<BenchmarkResult> results;

    template<typename Heap>
    static BenchmarkResult run_dijkstra(const Graph& g, const std::string& graph_type) {
        int n = g.size();
        const double INF = std::numeric_limits<double>::infinity();

        std::vector<double> dist(n, INF);
        std::vector<int> parent(n, -1);
        std::vector<typename Heap::Handle*> handles(n, nullptr);
        std::vector<bool> visited(n, false);

        Heap heap;

        auto start = std::chrono::high_resolution_clock::now();

        dist[0] = 0.0;
        handles[0] = heap.insert(0, 0.0);
        for (int v = 1; v < n; v++)
            handles[v] = heap.insert(v, INF);

        while (!heap.empty()) {
            auto [u, d] = heap.extract_min();
            visited[u] = true;
            if (d == INF) break;

            for (const auto& edge : g.adj[u]) {
                int v = edge.to;
                if (!visited[v]) {
                    double new_dist = dist[u] + edge.weight;
                    if (new_dist < dist[v]) {
                        dist[v] = new_dist;
                        parent[v] = u;
                        heap.decrease_key(handles[v], new_dist);
                    }
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        double total_ms = std::chrono::duration<double, std::milli>(end - start).count();

        double dist_sum = 0;
        for (double d : dist)
            if (d != INF) dist_sum += d;

        BenchmarkResult res;
        res.algorithm = "Dijkstra";
        res.heap_type = Heap::name();
        res.graph_type = graph_type;
        res.num_vertices = n;
        res.num_edges = g.edge_count();
        res.total_time_ms = total_ms;
        res.insert_ops = heap.insert_count;
        res.extract_min_ops = heap.extract_min_count;
        res.decrease_key_ops = heap.decrease_key_count;
        res.extract_min_time_ms = heap.extract_min_time_ns / 1e6;
        res.decrease_key_time_ms = heap.decrease_key_time_ns / 1e6;
        res.result_value = dist_sum;

        results.push_back(res);
        return res;
    }

    template<typename Heap>
    static BenchmarkResult run_prim(const Graph& g, const std::string& graph_type) {
        int n = g.size();
        const double INF = std::numeric_limits<double>::infinity();

        std::vector<double> key(n, INF);
        std::vector<int> parent(n, -1);
        std::vector<typename Heap::Handle*> handles(n, nullptr);
        std::vector<bool> in_mst(n, false);

        Heap heap;

        auto start = std::chrono::high_resolution_clock::now();

        key[0] = 0.0;
        handles[0] = heap.insert(0, 0.0);
        for (int v = 1; v < n; v++)
            handles[v] = heap.insert(v, INF);

        double total_weight = 0.0;

        while (!heap.empty()) {
            auto [u, d] = heap.extract_min();
            in_mst[u] = true;
            if (d == INF) break;
            total_weight += d;

            for (const auto& edge : g.adj[u]) {
                int v = edge.to;
                if (!in_mst[v] && edge.weight < key[v]) {
                    key[v] = edge.weight;
                    parent[v] = u;
                    heap.decrease_key(handles[v], edge.weight);
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        double total_ms = std::chrono::duration<double, std::milli>(end - start).count();

        BenchmarkResult res;
        res.algorithm = "Prim";
        res.heap_type = Heap::name();
        res.graph_type = graph_type;
        res.num_vertices = n;
        res.num_edges = g.edge_count();
        res.total_time_ms = total_ms;
        res.insert_ops = heap.insert_count;
        res.extract_min_ops = heap.extract_min_count;
        res.decrease_key_ops = heap.decrease_key_count;
        res.extract_min_time_ms = heap.extract_min_time_ns / 1e6;
        res.decrease_key_time_ms = heap.decrease_key_time_ns / 1e6;
        res.result_value = total_weight;

        results.push_back(res);
        return res;
    }

    static void print_header() {
        std::cout << std::left
                  << std::setw(10) << "Algo"
                  << std::setw(16) << "Heap"
                  << std::setw(14) << "Graph"
                  << std::setw(8)  << "|V|"
                  << std::setw(10) << "|E|"
                  << std::setw(12) << "Time(ms)"
                  << std::setw(9)  << "Inserts"
                  << std::setw(9)  << "ExtMin"
                  << std::setw(9)  << "DecKey"
                  << std::setw(13) << "ExtMin(ms)"
                  << std::setw(13) << "DecKey(ms)"
                  << std::setw(14) << "Result"
                  << std::endl;
        std::cout << std::string(127, '-') << std::endl;
    }

    static void print_result(const BenchmarkResult& r) {
        std::cout << std::left
                  << std::setw(10) << r.algorithm
                  << std::setw(16) << r.heap_type
                  << std::setw(14) << r.graph_type
                  << std::setw(8)  << r.num_vertices
                  << std::setw(10) << r.num_edges
                  << std::setw(12) << std::fixed << std::setprecision(3) << r.total_time_ms
                  << std::setw(9)  << r.insert_ops
                  << std::setw(9)  << r.extract_min_ops
                  << std::setw(9)  << r.decrease_key_ops
                  << std::setw(13) << std::fixed << std::setprecision(3) << r.extract_min_time_ms
                  << std::setw(13) << std::fixed << std::setprecision(3) << r.decrease_key_time_ms
                  << std::setw(14) << std::fixed << std::setprecision(2) << r.result_value
                  << std::endl;
    }

    static void print_all_results() {
        print_header();
        for (const auto& r : results) {
            print_result(r);
        }
    }

    static void verify_results() {
        std::cout << "\n========== CORRECTNESS VERIFICATION ==========\n\n";
        bool all_ok = true;

        for (size_t i = 0; i < results.size(); i++) {
            for (size_t j = i + 1; j < results.size(); j++) {
                if (results[i].algorithm == results[j].algorithm &&
                    results[i].graph_type == results[j].graph_type &&
                    results[i].num_vertices == results[j].num_vertices) {
                    double diff = std::fabs(results[i].result_value - results[j].result_value);
                    if (diff > 0.01) {
                        std::cout << "  MISMATCH: " << results[i].algorithm
                                  << " on " << results[i].graph_type
                                  << " (V=" << results[i].num_vertices << "): "
                                  << results[i].heap_type << "=" << results[i].result_value
                                  << " vs " << results[j].heap_type << "=" << results[j].result_value
                                  << std::endl;
                        all_ok = false;
                    }
                }
            }
        }
        if (all_ok) {
            std::cout << "  All heap implementations produce consistent results. PASSED.\n";
        }
    }

    static void print_summary() {
        std::cout << "\n========== PERFORMANCE SUMMARY ==========\n\n";

        for (const std::string& algo : {"Dijkstra", "Prim"}) {
            std::cout << "--- " << algo << " ---\n";
            for (const auto& r : results) {
                if (r.algorithm == algo) {
                    std::cout << "  " << std::setw(14) << std::left << r.heap_type
                              << " | " << std::setw(12) << r.graph_type
                              << " | V=" << std::setw(6) << r.num_vertices
                              << " E=" << std::setw(10) << r.num_edges
                              << " | " << std::setw(10) << std::fixed << std::setprecision(3)
                              << r.total_time_ms << " ms"
                              << " | dk=" << r.decrease_key_ops
                              << " em=" << r.extract_min_ops
                              << "\n";
                }
            }
            std::cout << "\n";
        }
    }
};

std::vector<BenchmarkResult> Benchmark::results;
