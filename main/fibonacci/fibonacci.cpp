#include <iostream>
#include <utility>

template <typename keytype>
struct Node {
    std::pair<keytype, int> key; // key is a pair of the value and its degree
    int degree; // tracks the degree of the tree e.g. b0, b1, b2, etc.
    Node* parent;
    Node* child; // the b1, whereas the parent would be b0
    Node* left;
    Node* right;
    bool mark; // if parent lost a child
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
    std::pair<keytype, int> findMin();
    keytype extractMin();

    // void insert(keytype k);
    Node<keytype>* insert(std::pair<keytype, int> k);
    void merge(FibHeap<keytype> &H2); // optional, but left in the book 
    void decreaseKey(Node<keytype>* node, std::pair<keytype, int> k); // degree maintained when child is lost

    void printKey();

private:
    // member variables
    static const int MAX_DEGREE = 32;
    Node<keytype>* minimumNode; // pointer to minimum key; also root

    // member functions
    void deleteHeap(Node<keytype>* node);
    Node<keytype>* copyHeap(Node<keytype>* oldNode, Node<keytype>* parent);
    void consolidate(); // only called on extract-min
    void link(Node<keytype>* childNode, Node<keytype>* parentNode);
    void cut(Node<keytype>* childNode, Node<keytype>* parentNode); // helper for decreaseKey
    void cascadingCut(Node<keytype>* node); // helper for decreaseKey
    void printHeap(Node<keytype>* node);
};

template<typename keytype>
FibHeap<keytype>::FibHeap() {
    //default constructor
    //the heap should be empty
    // std::cout << "DEFAULT CONSTRUCTOR CALLED" << std::endl; // TODO: delete this later

    minimumNode = nullptr;
}

template<typename keytype>
FibHeap<keytype>::FibHeap(keytype *k, int s) {
    /*
     * constructor for the class
     * the heap should be built using the array k containing s items
     * heap should be constructed using repeated insertions
     */
    // std::cout << "OVERLOADED CONSTRUCTOR CALLED" << std::endl; // TODO: delete this later

    minimumNode = nullptr;

    // for (int i = 0; i < s; i++) {
    //     insert(k[i]);
    // }

    for (int i = 0; i < s; i++) {
        // Construct a pair using the array value and a default index/id
        std::pair<keytype, int> entry = {k[i], i}; 
        insert(entry);
    }
}

template<typename keytype>
FibHeap<keytype>::~FibHeap() {
    if (minimumNode != nullptr) {
        deleteHeap(minimumNode);
    }
}

template<typename keytype>
void FibHeap<keytype>::deleteHeap(Node<keytype>* startNode) {
    if (startNode == nullptr) return;

    // Break the circularity to make it a simple linear list
    startNode->left->right = nullptr; 
    
    Node<keytype>* current = startNode;
    while (current != nullptr) {
        Node<keytype>* next = current->right;
        
        // Recursively destroy the child list
        if (current->child != nullptr) {
            deleteHeap(current->child);
        }
        
        delete current;
        current = next;
    }
}

template<typename keytype>
Node<keytype>* FibHeap<keytype>::copyHeap(Node<keytype>* oldNode, Node<keytype>* parent) {
    // copy constructor
    // should create a deep copy of the old heap
    // std::cout << "COPY CONSTRUCTOR CALLED" << std::endl; // delete this later
    if (oldNode == nullptr) return nullptr;

    Node<keytype>* firstNew = nullptr;
    Node<keytype>* lastNew = nullptr;
    Node<keytype>* currOld = oldNode;

    do {
        // Create new node
        Node<keytype>* newNode = new Node<keytype>;
        newNode->key = currOld->key;
        newNode->degree = currOld->degree;
        newNode->mark = currOld->mark;
        newNode->parent = parent;
        
        // Recursive copy of children
        newNode->child = copyHeap(currOld->child, newNode);

        // Link into new circular list
        if (firstNew == nullptr) {
            firstNew = newNode;
            newNode->left = newNode;
            newNode->right = newNode;
        } else {
            newNode->left = lastNew;
            newNode->right = firstNew;
            lastNew->right = newNode;
            firstNew->left = newNode;
        }
        lastNew = newNode;
        
        currOld = currOld->right;
    } while (currOld != oldNode);

    return firstNew;
}

template<typename keytype>
FibHeap<keytype>& FibHeap<keytype>::operator=(const FibHeap &rhs) {
    /*
     * assignment operator
     * should create a deep copy of the right hand side heap
     * and return a reference to the left hand side heap
     */
    if (this != &rhs) {
        // Clean up existing memory
        if (minimumNode != nullptr) deleteHeap(minimumNode);
        
        // Perform deep copy
        if (rhs.minimumNode == nullptr) {
            minimumNode = nullptr;
        } else {
            minimumNode = copyHeap(rhs.minimumNode, nullptr);
            // After copying must find the new minimum node 
            // because the copyList logic just copies order.
        }
    }
    return *this;
}

template<typename keytype>
std::pair<keytype, int> FibHeap<keytype>::findMin() {
    // returns the minimum key in the heap
    // without modifying the heap
    return minimumNode->key;
}

template<typename keytype>
keytype FibHeap<keytype>::extractMin() {
    /*
     * removes the minimum key from the heap and returns the key
     * children of the minimum should be inserted
     * into the root list in place of the minimum
     * (smallest tree to largest tree)
     * and consolidate called starting with the first (smallest rank) child tree
     */
    if (minimumNode == nullptr) {
        return keytype(); // empty heap
    }

    Node<keytype>* z = minimumNode;
    keytype minKey = z->key.first; // extracting the key from the pair

    // promote children to the root list
    if (z->child != nullptr) {
        Node<keytype>* child = z->child;
        Node<keytype>* startChild = child;
        
        // Iterate through the circular child list to nullify parent pointers
        do {
            Node<keytype>* nextChild = child->right;
            
            // Add child to root list (left of minimumNode)
            child->left = minimumNode->left;
            child->right = minimumNode;
            minimumNode->left->right = child;
            minimumNode->left = child;
            
            child->parent = nullptr; 
            child = nextChild;
        } while (child != startChild);
    }

    // Remove z from the root list
    z->left->right = z->right;
    z->right->left = z->left;

    if (z == z->right) {
        // z was the only node in the root list
        minimumNode = nullptr;
    } else {
        // Set a temporary minimum to start consolidation
        minimumNode = z->right;
        consolidate();
    }

    delete z;
    return minKey;
}

template<typename keytype>
void FibHeap<keytype>::consolidate() {
    Node<keytype>* rootArray[MAX_DEGREE] = {nullptr};

    // Find all nodes in the root list first to avoid pointer issues during modification
    std::vector<Node<keytype>*> nodes;
    Node<keytype>* curr = minimumNode;
    if (curr == nullptr) return;

    do {
        nodes.push_back(curr);
        curr = curr->right;
    } while (curr != minimumNode);

    // Merge trees of equal degree
    for (Node<keytype>* w : nodes) {
        int d = w->degree;
        while (rootArray[d] != nullptr) {
            Node<keytype>* y = rootArray[d];
            if (y->key < w->key) { // Using std::pair comparison
                std::swap(w, y);
            }
            link(y, w); // w becomes parent of y
            rootArray[d] = nullptr;
            d++;
        }
        rootArray[d] = w;
    }

    // Reconstruct root list and find new minimum
    minimumNode = nullptr;
    for (int i = 0; i < MAX_DEGREE; i++) {
        if (rootArray[i] != nullptr) {
            if (minimumNode == nullptr) {
                minimumNode = rootArray[i];
                minimumNode->left = minimumNode;
                minimumNode->right = minimumNode;
            } else {
                rootArray[i]->left = minimumNode->left;
                rootArray[i]->right = minimumNode;
                minimumNode->left->right = rootArray[i];
                minimumNode->left = rootArray[i];
                if (rootArray[i]->key < minimumNode->key) {
                    minimumNode = rootArray[i];
                }
            }
        }
    }
}

template<typename keytype>
void FibHeap<keytype>::link(Node<keytype> *childNode, Node<keytype> *parentNode) {
    /*
    * Link child as a child of parent
    * Update degree of parent and make child a child of parent
    */

    // removing child from root list
    childNode->left->right = childNode->right;
    childNode->right->left = childNode->left;

    // make child node a child of parent node
    childNode->parent = parentNode;

    if (parentNode->child == nullptr) {
        // Parent had no children, create a new circular list
        parentNode->child = childNode;
        childNode->left = childNode;
        childNode->right = childNode;
    } else {
        // Splice childNode into the parent's existing circular child list
        childNode->left = parentNode->child;
        childNode->right = parentNode->child->right;
        parentNode->child->right->left = childNode;
        parentNode->child->right = childNode;
    }

    // update parent properties
    parentNode->degree++;
    childNode->mark = false; // new children are always unmarked
}

template<typename keytype>
Node<keytype>* FibHeap<keytype>::insert(std::pair<keytype, int> k) {
    /*
     * inserts the key k into the heap
     * inserts a new b0 tree to the left of the min root
     * does not call consolidate
     */
    Node<keytype>* newNode = new Node<keytype>;
    newNode->key = k;
    newNode->degree = 0;
    newNode->parent = nullptr;
    newNode->child = nullptr;
    newNode->mark = false;

    if (minimumNode == nullptr) {// empty heap
        minimumNode = newNode;
        newNode->left = newNode;
        newNode->right = newNode;
    } else { // add new node to left of min root
        newNode->left = minimumNode->left;
        newNode->right = minimumNode;

        minimumNode->left->right = newNode;
        minimumNode->left = newNode;

        // if new key is smaller than min, update min
        if (k < minimumNode->key) {
            minimumNode = newNode;
        }
    }
    return newNode;
}

// Not part of the scope, but something leftover from before
template<typename keytype>
void FibHeap<keytype>::merge(FibHeap<keytype> &H2) {
    /*
     * merges the heap H2 into the current heap
     * consumes H2, H2 should be empty after
     * H2's root list should be inserted
     * after min in H1, with min in H2 being the first root inserted
     */

    if (H2.minimumNode == nullptr) {
        return; // empty heap
    }

    Node<keytype>* H2MinLeft = H2.minimumNode->left; // acts like a TEMP holding the original node

    H2MinLeft->right = minimumNode; // updates H2's left of min's right pointer to H1's min node
    H2.minimumNode->left = minimumNode->left; // updates's H2's min's left node to H1's min left pointer

    minimumNode->left->right = H2.minimumNode; // updates H1's left of min's right pointer to H2 min
    minimumNode->left = H2MinLeft; // updates H1's min's left pointer to H2's min's left pointer

    // effectively deletes H2 by disconnecting H2's min
    H2.minimumNode = nullptr;
}

template<typename keytype>
void FibHeap<keytype>::decreaseKey(Node<keytype>* node, std::pair<keytype, int> k) {
    /*
     * decreases the key of the given node to k
     * if the new key is greater than the old key, do nothing
     * if the new key is less than the old key, update the key and
     * if the new key is less than the parent's key, cut the node from its parent and
     * add it to the root list
     */
    
    if (k > node->key) {
        return; 
    }

    node->key = k;
    Node<keytype>* parentNode = node->parent;

    // If the node is not a root and violates the heap property
    if (parentNode != nullptr && node->key < parentNode->key) {
        cut(node, parentNode);
        cascadingCut(parentNode);
    }

    // Update the minimum pointer if the decreased key is the new global minimum
    if (node->key < minimumNode->key) {
        minimumNode = node;
    }
}

template<typename keytype>
void FibHeap<keytype>::cut(Node<keytype>* childNode, Node<keytype>* parentNode) {
    // Remove childNode from the cyclic child list of parentNode
    if (childNode->right == childNode) {
        // childNode was the only child
        parentNode->child = nullptr;
    } else {
        childNode->left->right = childNode->right;
        childNode->right->left = childNode->left;
        if (parentNode->child == childNode) {
            parentNode->child = childNode->right;
        }
    }
    parentNode->degree--;

    // Add childNode to the root list (to the left of minimumNode)
    childNode->left = minimumNode->left;
    childNode->right = minimumNode;
    minimumNode->left->right = childNode;
    minimumNode->left = childNode;

    childNode->parent = nullptr;
    childNode->mark = false; // Reset mark because it's now a root
}

template<typename keytype>
void FibHeap<keytype>::cascadingCut(Node<keytype>* node) {
    Node<keytype>* parentNode = node->parent;
    if (parentNode != nullptr) {
        if (!node->mark) {
            // If the node hasn't lost a child since it was last made a child of parent, mark it
            node->mark = true;
        } else {
            // If it was already marked, cut it and continue up the tree
            cut(node, parentNode);
            cascadingCut(parentNode);
        }
    }
}

template<typename keytype>
void FibHeap<keytype>::printKey() {
    /*
     * writes the stored in the heap
     * prints the tree with minimum first
     * the proceeds through the root list
     * printing each tree
     * when printing a binomial tree,
     * print the size of the tree first
     * and then use a modified preorder traversal of the tree
     */

    if (minimumNode == nullptr) return; // empty heap

    Node<keytype>* currentNode = minimumNode;
    do {
        std::cout << "B" << currentNode->degree << ":" << std::endl; // this will only print the first
        printHeap(currentNode);
        std::cout << std::endl;
        currentNode = currentNode->right;
    } while (currentNode != minimumNode);
}

template<typename keytype>
void FibHeap<keytype>::printHeap(Node<keytype>* node) {
    // Base case: If the node is null, return
    if (node == nullptr) {
        return;
    }
    // Print the key of the current node
    std::cout << node->key << " ";

    // Print the children of the current node
    Node<keytype>* childNode = node->child;
    while (childNode != nullptr) {
        printHeap(childNode);
        childNode = childNode->right;
        if (childNode == node->child) break; // Break the loop if we've reached the end of the children list
    }
}