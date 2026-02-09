#include <iostream>
#include <limits>
using namespace std;


// building the actual node structure of the heap establishes key, left child, and next sibling
struct HeapNode{
    int key;
    HeapNode *leftchild;
    HeapNode *nextSibling;
    HeapNode *parent;

    HeapNode():
    leftchild(NULL), nextSibling(NULL), parent(NULL) {}

    //create a new node
    HeapNode(int key_, HeapNode *leftchild_, HeapNode *nextSibling_):
    key(key_), leftchild(leftchild_), nextSibling(nextSibling_), parent(NULL) {}

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
bool Empty(HeapNode *node){
    return (node == NULL);
}

//function that is used to merge the two heaps
HeapNode *Merge(HeapNode *A, HeapNode *B){
    //return the non-empty node
    if(A == NULL) return B;
    if(B == NULL) return A;

    //maintain min heap, compare nodes with min val
    if(A->key < B->key){
        A->addChild(B);
        return A;
    }
    else{
        B->addChild(A);
        return B;
    }

    return NULL; //if unreachable
}
//return the key of the root node
//This only calls on non-empty heaps. Check Empty() first
//This is also the find min operation for the heap
int Top(HeapNode *node){
    return node->key;
}
//insert function, 
HeapNode *Insert(HeapNode *root, int key){
    HeapNode *newNode = new HeapNode(key, NULL, NULL);
    return newNode;
}
//decrease key function. this will decrease the key of a given node and restructure the heap
HeapNode *DecreaseKey(HeapNode *root, HeapNode *node, int newKey){
    if(node == NULL || newKey > node->key)
    return root; 

    node->key = newKey;

    //if node is root, no need for restructuring
    if(node->parent == NULL)
    return node;

    // if node is not root detatch the node from its current position
    if(node->parent->leftchild == node){
        node->parent->leftchild = node->nextSibling;
    }
    //if node is not the left child we need to find the sibling and update the pointer
    else{
        HeapNode *sibling = node->parent->leftchild;
        while(sibling != NULL && sibling->nextSibling != node)
        sibling = sibling->nextSibling;
        if(sibling != NULL)
        sibling->nextSibling = node->nextSibling;
    }

    node->nextSibling = NULL;   //detach the node from its current position
    node->parent = NULL; //detach the node from its current position

    //merge the detached subtree back with root
    return Merge(root, node);
}
//two pass merge function, used for delete operation
HeapNode *TwoPassMerge(HeapNode *node){
    if(node == NULL || node->nextSibling == NULL)
    return node;
else{
    HeapNode *A, *B, *newNode;
    A = node;
    B = node->nextSibling;
    newNode = node->nextSibling->nextSibling;

    A->nextSibling = NULL;
    B->nextSibling = NULL;

    return Merge(Merge(A, B), TwoPassMerge(newNode));
}
}

//delete the root node of the heap
HeapNode *Delete(HeapNode *node){
    return TwoPassMerge(node->leftchild);

}

//definitions: change struct to class

class PairingHeap{
public:
    HeapNode *root;

    PairingHeap():
        root(NULL) {}

    bool Empty(void) {
        return ::Empty(root);
    }

    int Top(void) {
        return ::Top(root);
    }


    HeapNode *Insert(int key) {
        HeapNode *newNode = ::Insert(root, key);
        root = ::Merge(root, newNode);
        return newNode;
    }

    //for implementation you can now do:
    /*Pairing heap;
    HeapNode *nodeA = heap.Insert(5);
    HeapNode *nodeB = heap.Insert(10);
    heap.DecreaseKey(nodeA, 3);*/


    void Delete(void) {
        root = ::Delete(root);
    }

    void Join(PairingHeap &other) {
        root = ::Merge(root, other.root);
    }

    HeapNode *DecreaseKey(HeapNode *node, int newKey) {
        root = ::DecreaseKey(root, node, newKey);
        return node;
    }
};

