#pragma once

struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    // isLeaf() function declaration
    bool isLeaf() const;

    // Update constructor
    HuffmanNode(char d, int f = 0) : character(d), frequency(f), left(nullptr), right(nullptr) {}

};
