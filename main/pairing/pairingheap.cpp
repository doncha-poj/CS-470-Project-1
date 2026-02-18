//Objective: Make a modular pairing heap structure so priority queue implementations can be swapped without changing algorithim logic
//Pairing heap needs to support: insert (this should return the node on insert for decrease key), extract min, decrease key, find-min
#include "pairingheap.hpp"

using namespace std;


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

HeapNode *Insert(HeapNode *root, int key, int value){
    ///fixed for compaitability issues now you can store references to the nodes for later decrease key operations
    HeapNode *newNode = new HeapNode(key, value, NULL, NULL);
    return newNode;
}

//Decrease key function
//Decreases the key of a given node and restructures the heap

HeapNode *DecreaseKey(HeapNode *root, HeapNode *node, int newKey){
    if(newKey > node->key)
        return node; 
    
    node->key = newKey;
    
    //If node is root, we don't need to restructure it we just need to return it
    if(node->parent == NULL)
        return node;
    
    
    if(node->parent->leftchild == node){
        node->parent->leftchild = node->nextSibling;
    }
    else{
        HeapNode *sibling = node->parent->leftchild;
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
    return TwoPassMerge(node->leftchild);

}

PairingHeap::~PairingHeap(){}

///Defining the Parining Heap
//Change to class for implementation of member functions

bool PairingHeap::empty() { return ::Empty(root); }

int PairingHeap::Top() { return :: Top(root); }

void PairingHeap::insert(int key, int value) {
    HeapNode *newNode = ::Insert(root, key, value);
    root = ::Merge(root, newNode);
    nodes.insert({value, newNode});
}

void PairingHeap::Delete() {
    nodes.erase(root->value);
    root = ::Delete(root);
}

void PairingHeap::Join(PairingHeap &other) { root = ::Merge(root, other.root); }

void PairingHeap::decreaseKey(int targ, int newKey) {
        HeapNode* node = nodes[targ];
        nodes.erase(targ);
        nodes.insert(targ, newKey);
        root = ::DecreaseKey(root, node, newKey);
}

int PairingHeap::extractMin() {
    int min = root->value;
    Delete();
    return min;
}

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

