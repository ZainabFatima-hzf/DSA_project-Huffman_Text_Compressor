#pragma once
#include "HuffmanNode.h"
#include <unordered_map>
#include <string>

class HuffmanTree {
private:
    HuffmanNode* root;
    std::unordered_map<char, std::string> codes;

    void buildCodes(HuffmanNode* node, const std::string& code);

public:
    HuffmanTree() : root(nullptr) {}
    HuffmanTree(const std::unordered_map<char,int>& freqMap);

    HuffmanNode* getRoot() { return root; }
    std::unordered_map<char, std::string> getCodes() { return codes; }

    std::string serializeTree();
    void deserializeTree(const std::string& data);

    char decodeNextBit(int bit, HuffmanNode*& current);
};
