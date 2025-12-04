#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <vector>
#include "../models/HuffmanNode.h"

class MinHeap {
private:
    std::vector<HuffmanNode*> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    MinHeap();
    void insert(HuffmanNode* node);
    HuffmanNode* extractMin();
    int size() const;
    bool empty() const;
};

#endif
