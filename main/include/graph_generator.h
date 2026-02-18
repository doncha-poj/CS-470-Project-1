#pragma once
#include "graph.h"
#include <random>
#include <algorithm>
#include <cmath>

class GraphGenerator {
public:
    // Erdos-Renyi random graph with given edge probability
    static Graph random_graph(int n, double edge_probability, int seed = 42) {
        Graph g(n);
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> prob(0.0, 1.0);
        std::uniform_real_distribution<double> weight(1.0, 100.0);

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (prob(rng) < edge_probability) {
                    double w = weight(rng);
                    g.add_undirected_edge(i, j, w);
                }
            }
        }
        ensure_connected(g, rng, weight);
        return g;
    }

    // Sparse random graph: ~4 edges per vertex on average
    static Graph sparse_random(int n, int seed = 42) {
        double p = 4.0 / n;
        return random_graph(n, p, seed);
    }

    // Dense random graph: ~50% of all possible edges present
    static Graph dense_random(int n, int seed = 42) {
        double p = 0.5;
        return random_graph(n, p, seed);
    }

    // 2D grid graph (rows x cols)
    static Graph grid_graph(int rows, int cols, int seed = 42) {
        int n = rows * cols;
        Graph g(n);
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> weight(1.0, 100.0);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int id = r * cols + c;
                if (c + 1 < cols)
                    g.add_undirected_edge(id, id + 1, weight(rng));
                if (r + 1 < rows)
                    g.add_undirected_edge(id, id + cols, weight(rng));
            }
        }
        return g;
    }

    // Worst-case graph: designed to force many decrease-key operations
    // Chain backbone + high-weight shortcuts from source that get improved
    static Graph worst_case_graph(int n, int seed = 42) {
        Graph g(n);
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> weight(50.0, 100.0);

        // Chain backbone with unit weights
        for (int i = 0; i < n - 1; i++) {
            g.add_undirected_edge(i, i + 1, 1.0);
        }

        // Cross-edges to nearby non-adjacent vertices
        for (int i = 0; i < n; i++) {
            for (int j = i + 2; j < std::min(i + 10, n); j++) {
                g.add_undirected_edge(i, j, weight(rng));
            }
        }

        // Shortcut edges from vertex 0 to every vertex with decreasing high weights
        // Forces initial discovery at high cost, then repeated decrease-key via chain
        for (int i = 2; i < n; i++) {
            g.add_undirected_edge(0, i, static_cast<double>(n - i + 1) * 10.0);
        }

        return g;
    }

    // Complete graph (all pairs connected)
    static Graph complete_graph(int n, int seed = 42) {
        Graph g(n);
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> weight(1.0, 100.0);

        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                g.add_undirected_edge(i, j, weight(rng));
        return g;
    }

private:
    // Ensure graph is connected by adding spanning-path edges where missing
    static void ensure_connected(Graph& g, std::mt19937& rng,
                                  std::uniform_real_distribution<double>& weight) {
        for (int i = 0; i < g.num_vertices - 1; i++) {
            bool found = false;
            for (const auto& e : g.adj[i]) {
                if (e.to == i + 1) { found = true; break; }
            }
            if (!found) {
                g.add_undirected_edge(i, i + 1, weight(rng));
            }
        }
    }
};
