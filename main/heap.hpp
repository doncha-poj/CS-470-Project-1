#pragma once
#include <utility>

class Heap {    // the parent class I need the heaps to inherit from
public:         // has all the methods I'll need to use, just make sure they're implemented with these signatures
    virtual ~Heap() = default;
    virtual void insert(int key, int value) = 0;
    virtual int extractMin() = 0;
    virtual void decreaseKey(int targ, int newKey) = 0;
    virtual bool empty() = 0;
};