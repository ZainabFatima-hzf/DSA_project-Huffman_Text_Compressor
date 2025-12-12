#include "HuffmanTree.h"
#include <queue>
#include <functional>
#include <unordered_map>
#include <vector>

// NOTE: You must include HuffmanNode constructor definitions here
// if they are not in the header (e.g., HuffmanNode::HuffmanNode(char c, int f) { ... })

// Implementation for the isLeaf helper function
bool HuffmanNode::isLeaf() const {
    return (left == nullptr && right == nullptr);
}

// ALL other HuffmanTree:: methods that were previously in this file MUST BE DELETED.
