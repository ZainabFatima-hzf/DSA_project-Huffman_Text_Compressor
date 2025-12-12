#include "HuffmanTree.h"
#include <queue>
#include <functional>
#include <stack>
#include <iostream>
#include <unordered_map>
#include <vector>

// Constructor: build Huffman Tree from frequency map
HuffmanTree::HuffmanTree(const std::unordered_map<char,int>& freqMap) {
    // Corrected member name used is 'frequency'
    auto cmp = [](HuffmanNode* a, HuffmanNode* b) { return a->frequency > b->frequency; };
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(cmp)> pq(cmp);

    for (auto& p : freqMap)
        pq.push(new HuffmanNode(p.first, p.second));

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        // Corrected member name used is 'frequency'
        HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
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
        // Corrected member name used is 'character'
        codes[node->character] = code;
    }
    buildCodes(node->left, code + "0");
    buildCodes(node->right, code + "1");
}

// Serialize tree using preorder traversal, '#' for nulls
std::string HuffmanTree::serializeTree() {
    std::string result;
    std::function<void(HuffmanNode*)> preorder = [&](HuffmanNode* node) {
        if (!node) { result += "#"; return; }
        // Corrected member name used is 'character'
        result += node->character;
        preorder(node->left);
        preorder(node->right);
    };
    preorder(root);
    return result;
}

// Deserialize tree from string
// Deserialize tree from string - CORRECTED VERSION
void HuffmanTree::deserializeTree(const std::string& serialized) {
    if (serialized.empty()) {
        root = nullptr;
        return;
    }

    size_t index = 0;
    std::function<HuffmanNode*()> build = [&]() -> HuffmanNode* {
        // Check bounds
        if (index >= serialized.size()) {
            std::cerr << "ERROR: Unexpected end of serialized string\n";
            return nullptr;
        }

        // Check for null marker
        if (serialized[index] == '#') {
            index++;
            return nullptr;
        }

        // Create node with character
        HuffmanNode* node = new HuffmanNode(serialized[index++]);

        // Recursively build left and right subtrees
        node->left = build();
        node->right = build();

        return node;
    };

    root = build();

    // Clear and rebuild codes
    codes.clear();
    if (root) {
        buildCodes(root, "");
    }
}

// Decode a bit using current traversal pointer
char HuffmanTree::decodeNextBit(int bit, HuffmanNode*& current) {
    if (!current) return '\0';
    current = (bit == 0) ? current->left : current->right;
    if (!current->left && !current->right) { // leaf
        // Corrected member name used is 'character'
        char val = current->character;
        current = root; // reset to root
        return val;
    }
    return '\0';
}

const std::unordered_map<char, std::string>& HuffmanTree::getCodes() const {
    return codes;
}
