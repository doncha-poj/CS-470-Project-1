#pragma once
#include <vector>
#include <utility>

struct Edge {
    int to;
    double weight;
};

class Graph {
public:
    int num_vertices;
    std::vector<std::vector<Edge>> adj;

    Graph(int n) : num_vertices(n), adj(n) {}

    void add_edge(int from, int to, double weight) {
        adj[from].push_back({to, weight});
    }

    void add_undirected_edge(int from, int to, double weight) {
        adj[from].push_back({to, weight});
        adj[to].push_back({from, weight});
    }

    int size() const { return num_vertices; }

    int edge_count() const {
        int count = 0;
        for (const auto& edges : adj)
            count += static_cast<int>(edges.size());
        return count;
    }
};
