#include "Compressor.h"
#include "../utils/BitsStreamWriter.h"
#include "../utils/FileIO.h"
#include "HuffmanTree.h"
#include <fstream>
#include <unordered_map>
#include <iostream>

void Compressor::compressFile(const std::string &inputFile, const std::string &outputFile) {
    // Step 1: read file & build frequency table
    std::vector<unsigned char> inputData = FileIO::readFile(inputFile);
    if (inputData.empty()) {
        std::cerr << "Input file is empty or cannot be opened: " << inputFile << std::endl;
        return;
    }

    std::unordered_map<char, int> freqMap;
    for (unsigned char ch : inputData) {
        freqMap[ch]++;
    }

    // Step 2: build Huffman Tree
    HuffmanTree tree(freqMap);
    std::string serializedTree = tree.serializeTree();
    int treeSize = serializedTree.size();

    // Step 3: write serialized tree to output file
    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout) {
        std::cerr << "Cannot open output file: " << outputFile << std::endl;
        return;
    }
    fout.write(reinterpret_cast<char*>(&treeSize), sizeof(treeSize));
    fout.write(serializedTree.c_str(), treeSize);
    fout.close();

    // Step 4: write compressed bits using BitStreamWriter
    BitStreamWriter writer(outputFile); // Open output file in binary mode
    auto codes = tree.getCodes();
    for (unsigned char ch : inputData) {
        writer.writeBits(codes[ch]);
    }
    writer.flush(); // Flush remaining bits

    std::cout << "Compression done! Output file: " << outputFile << std::endl;
}
