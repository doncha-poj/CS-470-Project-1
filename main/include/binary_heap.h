#pragma once
// Binary Heap (min-heap) baseline for graph algorithms
// Supports: insert (returns handle), extract-min, decrease-key, find-min
// Includes built-in metrics tracking for benchmarking

#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>

class BinaryHeap {
public:
    struct Node {
        double key;
        int vertex;
        int index; // position in the heap array

        Node(double k, int v, int i) : key(k), vertex(v), index(i) {}
    };

    using Handle = Node;

    // Metrics
    int insert_count = 0;
    int extract_min_count = 0;
    int decrease_key_count = 0;
    double extract_min_time_ns = 0;
    double decrease_key_time_ns = 0;

    BinaryHeap() {}

    ~BinaryHeap() {
        for (auto* node : heap)
            delete node;
    }

    // Insert a vertex with given priority; returns a handle for decrease-key
    Handle* insert(int vertex, double priority) {
        insert_count++;
        int idx = static_cast<int>(heap.size());
        Node* node = new Node(priority, vertex, idx);
        heap.push_back(node);
        sift_up(idx);
        return node;
    }

    // Remove and return the minimum element as (vertex, priority)
    std::pair<int, double> extract_min() {
        auto start = std::chrono::high_resolution_clock::now();
        extract_min_count++;

        Node* min_node = heap[0];
        int vertex = min_node->vertex;
        double key = min_node->key;

        int last = static_cast<int>(heap.size()) - 1;
        swap_nodes(0, last);
        heap.pop_back();
        delete min_node;

        if (!heap.empty())
            sift_down(0);

        auto end = std::chrono::high_resolution_clock::now();
        extract_min_time_ns += static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

        return {vertex, key};
    }

    // Decrease the priority of a previously inserted node
    void decrease_key(Handle* node, double new_key) {
        auto start = std::chrono::high_resolution_clock::now();
        decrease_key_count++;

        if (new_key >= node->key) {
            auto end = std::chrono::high_resolution_clock::now();
            decrease_key_time_ns += static_cast<double>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
            return;
        }

        node->key = new_key;
        sift_up(node->index);

        auto end = std::chrono::high_resolution_clock::now();
        decrease_key_time_ns += static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    bool empty() const { return heap.empty(); }
    int size() const { return static_cast<int>(heap.size()); }

    std::pair<int, double> find_min() const {
        return {heap[0]->vertex, heap[0]->key};
    }

    void reset_metrics() {
        insert_count = 0;
        extract_min_count = 0;
        decrease_key_count = 0;
        extract_min_time_ns = 0;
        decrease_key_time_ns = 0;
    }

    static const char* name() { return "BinaryHeap"; }

private:
    std::vector<Node*> heap;

    void swap_nodes(int i, int j) {
        std::swap(heap[i], heap[j]);
        heap[i]->index = i;
        heap[j]->index = j;
    }

    void sift_up(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[i]->key < heap[parent]->key) {
                swap_nodes(i, parent);
                i = parent;
            } else {
                break;
            }
        }
    }

    void sift_down(int i) {
        int sz = static_cast<int>(heap.size());
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < sz && heap[left]->key < heap[smallest]->key)
                smallest = left;
            if (right < sz && heap[right]->key < heap[smallest]->key)
                smallest = right;
            if (smallest != i) {
                swap_nodes(i, smallest);
                i = smallest;
            } else {
                break;
            }
        }
    }
};
