#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include <unordered_map>
// Include the necessary headers for the types used:
#include "../utils/BitsStreamWriter.h"
#include "../core/HuffmanTree.h"
#include "../models/HuffmanNode.h" // Required for the getTreeRoot() return type

class Compressor {
private:
    std::unordered_map<char, int> freqMap;
    std::unordered_map<char, std::string> codeMap;

    // CRITICAL FIX: Add the HuffmanTree object (this stores the tree)
    HuffmanTree tree;

public:
    void buildFrequencyTable(const std::string &filename);
    void buildHuffmanTree();
    void generateCodes();
    bool compressFile(const std::string &inputFile, const std::string &outputFile);

    // FIX C: Add the function needed by the GUI for visualization
    HuffmanNode* getTreeRoot() const;
};

#endif
