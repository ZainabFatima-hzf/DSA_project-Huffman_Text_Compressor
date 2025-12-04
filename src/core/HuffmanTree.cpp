#include "HuffmanTree.h"
#include <queue>
#include <iostream>

// Constructor: build Huffman Tree from frequency map
HuffmanTree::HuffmanTree(const std::unordered_map<char, int>& freqMap) {
    MinHeap minHeap;

    // Create leaf nodes and insert into heap
    for (auto pair : freqMap) {
        minHeap.insert(new HuffmanNode(pair.first, pair.second));
    }

    // Build tree
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.extractMin();
        HuffmanNode* right = minHeap.extractMin();
        HuffmanNode* parent = new HuffmanNode(left, right);
        minHeap.insert(parent);
    }

    root = minHeap.extractMin(); // root of Huffman Tree
}

// Get root (for GUI or decompression)
HuffmanNode* HuffmanTree::getRoot() const {
    return root;
}

// Recursive helper to generate codes
void HuffmanTree::generateCodes(HuffmanNode* node, const std::string& path,
                                std::unordered_map<char, std::string>& codes) const {
    if (!node) return;

    if (node->isLeaf()) {
        codes[node->character] = path;
    }

    generateCodes(node->left, path + "0", codes);
    generateCodes(node->right, path + "1", codes);
}

// Get character -> code map
std::unordered_map<char, std::string> HuffmanTree::getCodes() const {
    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);
    return codes;
}

// Print tree for console visualization (level-order)
void HuffmanTree::printTree() const {
    if (!root) return;
    std::queue<HuffmanNode*> q;
    q.push(root);

    while (!q.empty()) {
        HuffmanNode* node = q.front();
        q.pop();

        if (node->isLeaf())
            std::cout << node->character << ":" << node->frequency << " ";
        else
            std::cout << "*:" << node->frequency << " ";

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    std::cout << std::endl;
}

// Helper for tree serialization (pre-order)
void HuffmanTree::serializeHelper(HuffmanNode* node, std::string& out) const {
    if (!node) {
        return;
    }

    if (node->isLeaf()) {
        out += "L";
        out += node->character;
    } else {
        out += "I"; // internal node
    }

    serializeHelper(node->left, out);
    serializeHelper(node->right, out);
}

// Serialize tree
std::string HuffmanTree::serializeTree() const {
    std::string out;
    serializeHelper(root, out);
    return out;
}

// Helper for deserialization
HuffmanNode* HuffmanTree::deserializeHelper(const std::string& data, int& index) {
    if (index >= data.size()) return nullptr;
    char type = data[index++];

    if (type == 'L') {
        char ch = data[index++];
        return new HuffmanNode(ch, 0); // frequency irrelevant for decoding
    } else if (type == 'I') {
        HuffmanNode* leftNode = deserializeHelper(data, index);
        HuffmanNode* rightNode = deserializeHelper(data, index);
        return new HuffmanNode(leftNode, rightNode);
    }
    return nullptr;
}

// Deserialize tree
void HuffmanTree::deserializeTree(const std::string& data) {
    int index = 0;
    root = deserializeHelper(data, index);
}
