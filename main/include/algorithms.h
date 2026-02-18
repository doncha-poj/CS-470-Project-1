#pragma once
// Dijkstra's Shortest Path and Prim's MST algorithms
// Templated on heap type so priority queue implementations can be swapped
// without changing algorithm logic (satisfies Part 1 modularity requirement)

#include "graph.h"
#include <vector>
#include <limits>

struct DijkstraResult {
    std::vector<double> dist;
    std::vector<int> parent;
};

struct PrimResult {
    double total_weight;
    std::vector<int> parent;
};

// Dijkstra's single-source shortest path (non-negative weights)
// Heap type must support: insert(vertex, priority), extract_min(), decrease_key(handle, key), empty()
template<typename Heap>
DijkstraResult dijkstra(const Graph& g, int source) {
    int n = g.size();
    const double INF = std::numeric_limits<double>::infinity();

    DijkstraResult result;
    result.dist.assign(n, INF);
    result.parent.assign(n, -1);

    std::vector<typename Heap::Handle*> handles(n, nullptr);
    std::vector<bool> visited(n, false);

    Heap heap;
    result.dist[source] = 0.0;
    handles[source] = heap.insert(source, 0.0);

    for (int v = 0; v < n; v++) {
        if (v != source)
            handles[v] = heap.insert(v, INF);
    }

    while (!heap.empty()) {
        auto [u, d] = heap.extract_min();
        visited[u] = true;

        if (d == INF) break; // remaining vertices unreachable

        for (const auto& edge : g.adj[u]) {
            int v = edge.to;
            if (!visited[v]) {
                double new_dist = result.dist[u] + edge.weight;
                if (new_dist < result.dist[v]) {
                    result.dist[v] = new_dist;
                    result.parent[v] = u;
                    heap.decrease_key(handles[v], new_dist);
                }
            }
        }
    }

    return result;
}

// Prim's Minimum Spanning Tree algorithm
// Heap type must support: insert(vertex, priority), extract_min(), decrease_key(handle, key), empty()
template<typename Heap>
PrimResult prim(const Graph& g) {
    int n = g.size();
    const double INF = std::numeric_limits<double>::infinity();

    PrimResult result;
    result.total_weight = 0.0;
    result.parent.assign(n, -1);

    std::vector<double> key(n, INF);
    std::vector<typename Heap::Handle*> handles(n, nullptr);
    std::vector<bool> in_mst(n, false);

    Heap heap;
    key[0] = 0.0;
    handles[0] = heap.insert(0, 0.0);

    for (int v = 1; v < n; v++) {
        handles[v] = heap.insert(v, INF);
    }

    while (!heap.empty()) {
        auto [u, d] = heap.extract_min();
        in_mst[u] = true;

        if (d == INF) break; // remaining vertices not reachable
        result.total_weight += d;

        for (const auto& edge : g.adj[u]) {
            int v = edge.to;
            if (!in_mst[v] && edge.weight < key[v]) {
                key[v] = edge.weight;
                result.parent[v] = u;
                heap.decrease_key(handles[v], edge.weight);
            }
        }
    }

    return result;
}
