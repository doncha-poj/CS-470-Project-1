#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H
#include <iostream>
#include "../heap.hpp"

template <typename keytype>
struct Node {
    keytype key;
    int value;
    int degree; // tracks the degree of the tree e.g. b0, b1, b2, etc.
    Node* parent;
    Node* child; // the b1, whereas the parent would be b0
    Node* left;
    Node* right;
    bool mark; // if parent lost a child
};

template<typename keytype>
class FibHeap : public Heap {
public:
    // constructors
    FibHeap();
    FibHeap(keytype k[], int s);
    // rule of three
    ~FibHeap();
    FibHeap(const FibHeap &old);
    FibHeap& operator=(const FibHeap& rhs);

    // member functions
    keytype findMin();
    keytype extractMin();

    void insert(keytype key, int value) ;
    void merge(FibHeap<keytype> &H2); // optional, but left in the book 
    void decreaseKey(int targ, int newKey) ; // TODO: implement later
    bool empty();
    void printKey();

private:
    // member variables
    static const int MAX_DEGREE = 32;
    Node<keytype>* minimumNode; // pointer to minimum key; also root

    // member functions
    void deleteHeap(Node<keytype>* node);
    void consolidate(); // only called on extract-min
    void link(Node<keytype>* childNode, Node<keytype>* parentNode);
    void printHeap(Node<keytype>* node);
};
#endif