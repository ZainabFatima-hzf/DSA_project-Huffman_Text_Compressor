#pragma once

struct HuffmanNode {
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char d, int f = 0) : data(d), freq(f), left(nullptr), right(nullptr) {}
};
