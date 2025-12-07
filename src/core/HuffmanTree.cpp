#include "HuffmanTree.h"
#include <queue>
#include <functional>
#include <stack>
#include <iostream>

// Constructor: build Huffman Tree from frequency map
HuffmanTree::HuffmanTree(const std::unordered_map<char,int>& freqMap) {
    auto cmp = [](HuffmanNode* a, HuffmanNode* b) { return a->freq > b->freq; };
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(cmp)> pq(cmp);

    for (auto& p : freqMap)
        pq.push(new HuffmanNode(p.first, p.second));

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    root = pq.empty() ? nullptr : pq.top();
    codes.clear();
    buildCodes(root, "");
}

// Recursive helper to generate codes
void HuffmanTree::buildCodes(HuffmanNode* node, const std::string& code) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->data] = code;
    }
    buildCodes(node->left, code + "0");
    buildCodes(node->right, code + "1");
}

// Serialize tree using preorder traversal, '#' for nulls
std::string HuffmanTree::serializeTree() {
    std::string result;
    std::function<void(HuffmanNode*)> preorder = [&](HuffmanNode* node) {
        if (!node) { result += "#"; return; }
        result += node->data;
        preorder(node->left);
        preorder(node->right);
    };
    preorder(root);
    return result;
}

// Deserialize tree from string
void HuffmanTree::deserializeTree(const std::string& data) {
    size_t index = 0;
    std::function<HuffmanNode*()> build = [&]() -> HuffmanNode* {
        if (index >= data.size() || data[index] == '#') { index++; return nullptr; }
        HuffmanNode* node = new HuffmanNode(data[index++]);
        node->left = build();
        node->right = build();
        return node;
    };
    root = build();
    codes.clear();
    buildCodes(root, "");
}

// Decode a bit using current traversal pointer
char HuffmanTree::decodeNextBit(int bit, HuffmanNode*& current) {
    if (!current) return '\0';
    current = (bit == 0) ? current->left : current->right;
    if (!current->left && !current->right) { // leaf
        char val = current->data;
        current = root; // reset to root
        return val;
    }
    return '\0';
}
