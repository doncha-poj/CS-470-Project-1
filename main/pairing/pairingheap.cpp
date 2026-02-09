//Objective: Make a modular pairing heap structure so priority queue implementations can be swapped without changing algorithim logic
//Pairing heap needs to support: insert (this should return the node on insert for decrease key), extract min, decrease key, find-min


#include <iostream>
#include <limits>
using namespace std;

//This function builds the actual node structure of the heap
//Establishes key, left most child, and the next sibling over

struct HeapNode{
    int key; 
    HeapNode *leftChild; 
    HeapNode *nextSibling; 
    HeapNode *parent; //pointer to parent (for decrease key)

    HeapNode():
     leftChild(NULL), nextSibling(NULL), parent(NULL) {}

     //Need to create a new node

     HeapNode(int key_, HeapNode *leftChild_, HeapNode *nextSibling_):
      key(key_), leftChild(leftChild_), nextSibling(nextSibling_), parent(NULL) {}

      //Adds a child and sibling

      void addChild(HeapNode *child){
        if(leftChild == NULL)
        leftChild = child;
        else{
            child->nextSibling = leftChild;
            leftChild = child;
        }
        child->parent = this;
      }
};


//Return true if root of tree is null and false otherwise

bool Empty(HeapNode *node){
    return (node == NULL);
}

//Function that is used to merge the two heaps

HeapNode *Merge(HeapNode *A, HeapNode *B){
    
    //return the non-empty node
    if(A == NULL) return B;
    if(B == NULL) return A;

    //Maintain min heap, compare nodes with min val

    if(A->key < B->key){
        A->addChild(B);
        return A;
    }
    else{
        B->addChild(A);
        return B;
    }

    return NULL; //this is for if the node is unreachable.

}

//Returns the root value of the heap
//Note: Only call on non-empty heaps. Check Empty() first if unsure.
//in place of find_min function
int Top(HeapNode *node){
    return node->key;
}

//Insert function

HeapNode *Insert(int key){
    ///fixed for compaitability issues now you can store references to the nodes for later decrease key operations
    HeapNode *newNode = new HeapNode(key, NULL, NULL);
    return newNode;
}

//Decrease key function
//Decreases the key of a given node and restructures the heap

HeapNode *DecreaseKey(HeapNode *&root, HeapNode *node, int newKey){
    if(newKey > node->key)
        return node; 
    
    node->key = newKey;
    
    //If node is root, we don't need to restructure it we just need to return it
    if(node->parent == NULL)
        return node;
    
    
    if(node->parent->leftChild == node){
        node->parent->leftChild = node->nextSibling;
    }
    else{
        HeapNode *sibling = node->parent->leftChild;
        while(sibling != NULL && sibling->nextSibling != node)
            sibling = sibling->nextSibling;
        if(sibling != NULL)
            sibling->nextSibling = node->nextSibling;
    }
    
    node->nextSibling = NULL;
    node->parent = NULL;
    
    //Merge the detached subtree back with root
    root = Merge(root, node);
    return node;
}

//Delete root node

HeapNode *TwoPassMerge(HeapNode *node){
    if(node == NULL || node->nextSibling == NULL)
    return node;
else {
    HeapNode *A, *B, *newNode;
    A = node;
    B = node->nextSibling;
    newNode = node->nextSibling->nextSibling;

    A->nextSibling = NULL;
    B->nextSibling = NULL;


    return Merge(Merge(A, B), TwoPassMerge(newNode));

}
    //if unreachable
    return NULL;
}

//implementation of delete the root node in heap
//Note: Only call on non-empty heaps. Check Empty() first if unsure.

HeapNode *Delete(HeapNode *node){
    return TwoPassMerge(node->leftChild);

}

///Defining the Parining Heap
//Change to class for implementation of member functions

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
        HeapNode *newNode = ::Insert(key);
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
        return ::DecreaseKey(root, node, newKey);
    }
};

int main() {
    PairingHeap heap;
    
    // insert test
    HeapNode *node5 = heap.Insert(5);
    HeapNode *node10 = heap.Insert(10);
    HeapNode *node3 = heap.Insert(3);
    HeapNode *node7 = heap.Insert(7);
    
    cout << "Min value: " << heap.Top() << endl;
    
    // decrease key test
    heap.DecreaseKey(node10, 1);
    cout << "After decreasing 10 to 1, min value: " << heap.Top() << endl;
    
    // delete test
    heap.Delete();
    cout << "After deleting min and here is new min " << heap.Top() << endl;
    
    cout << "yes this is working lmao" << endl;
    
    return 0;
}

