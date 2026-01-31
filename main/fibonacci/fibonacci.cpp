#include <iostream>

template <typename keytype>
struct Node {
    keytype key;
    int degree; // tracks the degree of the tree e.g. deg0, deg1, deg2, etc.
    Node* parent;
    Node* child; // the deg1, whereas the parent would be deg0
    Node* left;
    Node* right;
};

template<typename keytype>
class FibHeap {
public:
    // constructors
    FibHeap();
    FibHeap(keytype k[], int s);
    // rule of three
    ~FibHeap();
    FibHeap(const FibHeap &old);
    FibHeap& operator=(const FibHeap& rhs);

    // member functions
    keytype peekKey();
    keytype extractMin();

    void insert(keytype k);
    void merge(FibHeap<keytype> &H2);
    void printKey();

private:
    // member variables
    static const int MAX_DEGREE = 32;
    Node<keytype>* minimumNode; // pointer to minimum key
};
