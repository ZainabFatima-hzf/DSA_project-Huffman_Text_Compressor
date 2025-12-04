#include "HuffmanNode.h"

// Constructor for leaf node
HuffmanNode::HuffmanNode(char ch, int freq)
    : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

// Constructor for internal node
HuffmanNode::HuffmanNode(HuffmanNode* leftNode, HuffmanNode* rightNode)
    : character('\0'), frequency(leftNode->frequency + rightNode->frequency),
      left(leftNode), right(rightNode) {}

// Check if node is a leaf
bool HuffmanNode::isLeaf() const {
    return (left == nullptr && right == nullptr);
}
