#pragma once
// Pairing Heap for graph algorithms
// Adapted from the implementation in main/pairing/pairingheap.cpp
// Supports: insert (returns handle), extract-min, decrease-key, find-min
// Includes built-in metrics tracking for benchmarking

#include <utility>
#include <chrono>

class PairingHeap {
public:
    struct Node {
        double key;
        int vertex;
        Node* left_child;
        Node* next_sibling;
        Node* parent;

        Node(double k, int v)
            : key(k), vertex(v),
              left_child(nullptr), next_sibling(nullptr), parent(nullptr) {}

        void add_child(Node* child) {
            if (!left_child) {
                left_child = child;
            } else {
                child->next_sibling = left_child;
                left_child = child;
            }
            child->parent = this;
        }
    };

    using Handle = Node;

    // Metrics
    int insert_count = 0;
    int extract_min_count = 0;
    int decrease_key_count = 0;
    double extract_min_time_ns = 0;
    double decrease_key_time_ns = 0;

    PairingHeap() : root(nullptr), n(0) {}

    ~PairingHeap() { delete_all(root); }

    // Insert a vertex with given priority; returns a handle for decrease-key
    Handle* insert(int vertex, double priority) {
        insert_count++;
        Node* node = new Node(priority, vertex);
        root = merge(root, node);
        n++;
        return node;
    }

    // Remove and return the minimum element as (vertex, priority)
    std::pair<int, double> extract_min() {
        auto start = std::chrono::high_resolution_clock::now();
        extract_min_count++;

        int vertex = root->vertex;
        double key = root->key;

        Node* old_root = root;
        root = two_pass_merge(root->left_child);
        if (root) root->parent = nullptr;
        delete old_root;
        n--;

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

        // If node is root, just update key
        if (node == root) {
            auto end = std::chrono::high_resolution_clock::now();
            decrease_key_time_ns += static_cast<double>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
            return;
        }

        // Detach node from its parent
        if (node->parent->left_child == node) {
            node->parent->left_child = node->next_sibling;
        } else {
            Node* sibling = node->parent->left_child;
            while (sibling && sibling->next_sibling != node)
                sibling = sibling->next_sibling;
            if (sibling)
                sibling->next_sibling = node->next_sibling;
        }

        node->next_sibling = nullptr;
        node->parent = nullptr;

        // Merge detached subtree back with root
        root = merge(root, node);

        auto end = std::chrono::high_resolution_clock::now();
        decrease_key_time_ns += static_cast<double>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
    }

    bool empty() const { return root == nullptr; }
    int size() const { return n; }

    std::pair<int, double> find_min() const {
        return {root->vertex, root->key};
    }

    void reset_metrics() {
        insert_count = 0;
        extract_min_count = 0;
        decrease_key_count = 0;
        extract_min_time_ns = 0;
        decrease_key_time_ns = 0;
    }

    static const char* name() { return "PairingHeap"; }

private:
    Node* root;
    int n;

    static Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->key <= b->key) {
            a->add_child(b);
            return a;
        } else {
            b->add_child(a);
            return b;
        }
    }

    // Two-pass merge (used in extract-min): pair up siblings left-to-right,
    // then merge resulting trees right-to-left
    static Node* two_pass_merge(Node* node) {
        if (!node || !node->next_sibling) return node;

        Node* a = node;
        Node* b = node->next_sibling;
        Node* rest = b->next_sibling;

        a->next_sibling = nullptr;
        a->parent = nullptr;
        b->next_sibling = nullptr;
        b->parent = nullptr;

        return merge(merge(a, b), two_pass_merge(rest));
    }

    void delete_all(Node* node) {
        if (!node) return;
        delete_all(node->left_child);
        delete_all(node->next_sibling);
        delete node;
    }
};
