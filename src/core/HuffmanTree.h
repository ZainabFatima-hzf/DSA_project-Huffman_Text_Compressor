#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <unordered_map>
#include <string>
#include "../models/HuffmanNode.h"
#include "../structures/MinHeap.h"

class HuffmanTree {
private:
    HuffmanNode* root;

    void generateCodes(HuffmanNode* node, const std::string& path,
                       std::unordered_map<char, std::string>& codes) const;

    void serializeHelper(HuffmanNode* node, std::string& out) const;
    HuffmanNode* deserializeHelper(const std::string& data, int& index);

public:
    HuffmanTree(const std::unordered_map<char, int>& freqMap);
 
    HuffmanTree() : root(nullptr) {}

    HuffmanNode* getRoot() const;

    std::unordered_map<char, std::string> getCodes() const;

    void printTree() const;

    // NEW: Required for file saving
    std::string serializeTree() const;

    // NEW: Required during decompression
    void deserializeTree(const std::string& data);
};

#endif
