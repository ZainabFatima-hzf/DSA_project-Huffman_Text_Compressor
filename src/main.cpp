#include <iostream>
#include <unordered_map>
#include <string>

#include "models/HuffmanNode.h"
#include "structures/MinHeap.h"
#include "core/HuffmanTree.h"

int main() {
    // Sample input text
    std::string text = "this is an example for huffman encoding";

    // 1️ Build frequency table
    std::unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

    // 2 Build Huffman Tree
    HuffmanTree huffmanTree(freqMap);

    // 3 Print the tree (level-order)
    std::cout << "Huffman Tree (Level-order): ";
    huffmanTree.printTree();
    std::cout << std::endl;

    // 4 Generate Huffman Codes
    std::unordered_map<char, std::string> codes = huffmanTree.getCodes();
    std::cout << "Huffman Codes:\n";
    for (auto pair : codes) {
        std::cout << "'" << pair.first << "' : " << pair.second << std::endl;
    }

    // 5 Serialize the tree
    std::string serializedTree = huffmanTree.serializeTree();
    std::cout << "\nSerialized Tree: " << serializedTree << std::endl;

    // 6 Deserialize into a new tree
    HuffmanTree newTree; // temporary
    newTree.deserializeTree(serializedTree);

    // 7 Print deserialized tree
    std::cout << "\nDeserialized Tree (Level-order): ";
    newTree.printTree();
    std::cout << std::endl;

    // 8 Generate codes from deserialized tree
    std::unordered_map<char, std::string> newCodes = newTree.getCodes();
    std::cout << "\nCodes from deserialized tree:\n";
    for (auto pair : newCodes) {
        std::cout << "'" << pair.first << "' : " << pair.second << std::endl;
    }

    return 0;
}
