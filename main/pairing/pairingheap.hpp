#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H
#include <iostream>
#include <limits>
#include <map>
#include "../heap.hpp"

using namespace std;

// building the actual node structure of the heap establishes key, left child, and next sibling
struct HeapNode{
    int key;    // for priority
    int value;  // for index in adjacency list
    HeapNode *leftchild;
    HeapNode *nextSibling;
    HeapNode *parent;

    HeapNode():
    leftchild(NULL), nextSibling(NULL), parent(NULL) {}

    //create a new node
    HeapNode(int key_, int value_, HeapNode *leftchild_, HeapNode *nextSibling_):
    key(key_), value(value_), leftchild(leftchild_), nextSibling(nextSibling_), parent(NULL) {}

    //add a child and sibling
    void addChild(HeapNode *child){
        if(leftchild == NULL)
        leftchild = child;
        else{
            child->nextSibling = leftchild;
            leftchild = child;
        }
        child->parent = this;
    }

};

//return true if root of tree is null and false otherwise
bool Empty(HeapNode *node);

//function that is used to merge the two heaps
HeapNode *Merge(HeapNode *A, HeapNode *B);
//return the key of the root node
//This only calls on non-empty heaps. Check Empty() first
//This is also the find min operation for the heap
int Top(HeapNode *node);

//insert function, 
HeapNode *Insert(HeapNode *root, int key, int value);

//decrease key function. this will decrease the key of a given node and restructure the heap
HeapNode *DecreaseKey(HeapNode *root, HeapNode *node, int newKey);

//two pass merge function, used for delete operation
HeapNode *TwoPassMerge(HeapNode *node);

//delete the root node of the heap
HeapNode *Delete(HeapNode *node);

//definitions: change struct to class

class PairingHeap : public Heap {
public:
    HeapNode *root;
    map<int, HeapNode*> nodes;

    PairingHeap():
        root(NULL) {}

    ~PairingHeap();
    bool empty() override;

    int Top();


    void insert(int key, int value) override;

    //for implementation you can now do:
    /*Pairing heap;
    HeapNode *nodeA = heap.Insert(5);
    HeapNode *nodeB = heap.Insert(10);
    heap.DecreaseKey(nodeA, 3);*/


    void Delete();

    void Join(PairingHeap &other);

    void decreaseKey(int targ, int newKey) override;

    int extractMin() override;
};

#endif