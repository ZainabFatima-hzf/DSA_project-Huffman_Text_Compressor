#include "MinHeap.h"
#include <algorithm> // for std::swap

MinHeap::MinHeap() {}

// Return size
int MinHeap::size() const {
    return heap.size();
}

// Check if empty
bool MinHeap::empty() const {
    return heap.empty();
}

// Swap up to maintain heap property
void MinHeap::heapifyUp(int index) {
    if (index == 0) return;
    int parent = (index - 1) / 2;
    if (heap[index]->frequency < heap[parent]->frequency) {
        std::swap(heap[index], heap[parent]);
        heapifyUp(parent);
    }
}

// Swap down to maintain heap property
void MinHeap::heapifyDown(int index) {
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    int smallest = index;

    if (leftChild < heap.size() && heap[leftChild]->frequency < heap[smallest]->frequency)
        smallest = leftChild;
    if (rightChild < heap.size() && heap[rightChild]->frequency < heap[smallest]->frequency)
        smallest = rightChild;

    if (smallest != index) {
        std::swap(heap[index], heap[smallest]);
        heapifyDown(smallest);
    }
}

// Insert node
void MinHeap::insert(HuffmanNode* node) {
    heap.push_back(node);
    heapifyUp(heap.size() - 1);
}

// Extract node with minimum frequency
HuffmanNode* MinHeap::extractMin() {
    if (heap.empty()) return nullptr;
    HuffmanNode* minNode = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) heapifyDown(0);
    return minNode;
}
