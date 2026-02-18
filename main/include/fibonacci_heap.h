#pragma once
// Fibonacci Heap for graph algorithms
// Adapted from the implementation in main/fibonacci/fibonacci.cpp
// Supports: insert (returns handle), extract-min, decrease-key, find-min
// Includes built-in metrics tracking for benchmarking

#include <vector>
#include <utility>
#include <limits>
#include <chrono>
#include <algorithm>

class FibonacciHeap {
public:
    struct Node {
        double key;
        int vertex;
        int degree;
        Node* parent;
        Node* child;
        Node* left;
        Node* right;
        bool mark;

        Node(double k, int v)
            : key(k), vertex(v), degree(0),
              parent(nullptr), child(nullptr),
              left(this), right(this), mark(false) {}
    };

    using Handle = Node;

    // Metrics
    int insert_count = 0;
    int extract_min_count = 0;
    int decrease_key_count = 0;
    double extract_min_time_ns = 0;
    double decrease_key_time_ns = 0;

    FibonacciHeap() : min_node(nullptr), n(0) {}

    ~FibonacciHeap() {
        if (min_node) delete_all(min_node);
    }

    // Insert a vertex with given priority; returns a handle for decrease-key
    Handle* insert(int vertex, double priority) {
        insert_count++;
        Node* node = new Node(priority, vertex);
        if (min_node == nullptr) {
            min_node = node;
        } else {
            // Splice into root list to the left of min
            node->left = min_node->left;
            node->right = min_node;
            min_node->left->right = node;
            min_node->left = node;
            if (node->key < min_node->key)
                min_node = node;
        }
        n++;
        return node;
    }

    // Remove and return the minimum element as (vertex, priority)
    std::pair<int, double> extract_min() {
        auto start = std::chrono::high_resolution_clock::now();
        extract_min_count++;

        Node* z = min_node;
        int vertex = z->vertex;
        double key = z->key;

        // Promote all children to root list
        if (z->child) {
            Node* child = z->child;
            Node* start_child = child;
            do {
                Node* next = child->right;
                child->left = min_node->left;
                child->right = min_node;
                min_node->left->right = child;
                min_node->left = child;
                child->parent = nullptr;
                child = next;
            } while (child != start_child);
        }

        // Remove z from root list
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right) {
            min_node = nullptr;
        } else {
            min_node = z->right;
            consolidate();
        }

        n--;
        delete z;

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
        Node* parent = node->parent;

        if (parent && node->key < parent->key) {
            cut(node, parent);
            cascading_cut(parent);
        }

        if (node->key < min_node->key)
            min_node = node;

        auto end = std::chrono::high_resolution_clock::now();
        decrease_key_time_ns += static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    bool empty() const { return min_node == nullptr; }
    int size() const { return n; }

    std::pair<int, double> find_min() const {
        return {min_node->vertex, min_node->key};
    }

    void reset_metrics() {
        insert_count = 0;
        extract_min_count = 0;
        decrease_key_count = 0;
        extract_min_time_ns = 0;
        decrease_key_time_ns = 0;
    }

    static const char* name() { return "FibonacciHeap"; }

private:
    static const int MAX_DEGREE = 45;
    Node* min_node;
    int n;

    void consolidate() {
        Node* degree_table[MAX_DEGREE] = {nullptr};

        // Collect all current root nodes to avoid pointer invalidation
        std::vector<Node*> roots;
        Node* curr = min_node;
        do {
            roots.push_back(curr);
            curr = curr->right;
        } while (curr != min_node);

        for (Node* w : roots) {
            int d = w->degree;
            while (d < MAX_DEGREE && degree_table[d]) {
                Node* y = degree_table[d];
                if (y->key < w->key) std::swap(w, y);
                link(y, w);
                degree_table[d] = nullptr;
                d++;
            }
            if (d < MAX_DEGREE)
                degree_table[d] = w;
        }

        // Rebuild root list and locate new minimum
        min_node = nullptr;
        for (int i = 0; i < MAX_DEGREE; i++) {
            if (degree_table[i]) {
                if (!min_node) {
                    min_node = degree_table[i];
                    min_node->left = min_node;
                    min_node->right = min_node;
                } else {
                    degree_table[i]->left = min_node->left;
                    degree_table[i]->right = min_node;
                    min_node->left->right = degree_table[i];
                    min_node->left = degree_table[i];
                    if (degree_table[i]->key < min_node->key)
                        min_node = degree_table[i];
                }
            }
        }
    }

    void link(Node* child, Node* parent) {
        // Remove child from root list
        child->left->right = child->right;
        child->right->left = child->left;

        child->parent = parent;
        if (!parent->child) {
            parent->child = child;
            child->left = child;
            child->right = child;
        } else {
            child->left = parent->child;
            child->right = parent->child->right;
            parent->child->right->left = child;
            parent->child->right = child;
        }
        parent->degree++;
        child->mark = false;
    }

    void cut(Node* child, Node* parent) {
        if (child->right == child) {
            parent->child = nullptr;
        } else {
            child->left->right = child->right;
            child->right->left = child->left;
            if (parent->child == child)
                parent->child = child->right;
        }
        parent->degree--;

        // Add child to root list
        child->left = min_node->left;
        child->right = min_node;
        min_node->left->right = child;
        min_node->left = child;
        child->parent = nullptr;
        child->mark = false;
    }

    void cascading_cut(Node* node) {
        Node* parent = node->parent;
        if (parent) {
            if (!node->mark) {
                node->mark = true;
            } else {
                cut(node, parent);
                cascading_cut(parent);
            }
        }
    }

    void delete_all(Node* start) {
        if (!start) return;
        start->left->right = nullptr; // break circularity
        Node* curr = start;
        while (curr) {
            Node* next = curr->right;
            if (curr->child) delete_all(curr->child);
            delete curr;
            curr = next;
        }
    }
};
