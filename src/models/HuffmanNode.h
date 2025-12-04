#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

class HuffmanNode {
public:
    char character;       // character stored (valid only in leaf nodes)
    int frequency;        // frequency or combined frequency
    HuffmanNode* left;    // left child
    HuffmanNode* right;   // right child

    // Constructor for leaf node
    HuffmanNode(char ch, int freq);

    // Constructor for internal node
    HuffmanNode(HuffmanNode* leftNode, HuffmanNode* rightNode);

    bool isLeaf() const;
};

#endif
