#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <string>
#include <unordered_map>
#include "../utils/BitsStreamWriter.h"
#include "../core/HuffmanTree.h" // include Member 1's class

class Compressor {
private:
    std::unordered_map<char, int> freqMap;
    std::unordered_map<char, std::string> codeMap;

public:
    void buildFrequencyTable(const std::string &filename);
    void buildHuffmanTree();
    void generateCodes();
    void compressFile(const std::string &inputFile, const std::string &outputFile);
};

#endif
