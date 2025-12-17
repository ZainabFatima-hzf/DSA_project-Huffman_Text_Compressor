#include "Compressor.h"
#include "../utils/FileIO.h"
#include "HuffmanTree.h"
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <sstream>

HuffmanNode* Compressor::getTreeRoot() const {
    return tree.getRoot();
}

bool Compressor::compressFile(const std::string &inputFile, const std::string &outputFile) {
    // Step 1: Read input file
    std::vector<unsigned char> inputData = FileIO::readFile(inputFile);
    if (inputData.empty()) {
        std::cerr << "ERROR: Input file is empty or cannot be opened: " << inputFile << std::endl;
        return false;
    }

    std::cout << "DEBUG: Read " << inputData.size() << " bytes from input file" << std::endl;

    // Step 2: Build frequency table
    freqMap.clear();
    for (unsigned char ch : inputData) {
        freqMap[ch]++;
    }

    std::cout << "DEBUG: Frequency table built with " << freqMap.size() << " unique characters" << std::endl;

    // Step 3: Build Huffman Tree
    tree = HuffmanTree(freqMap);

    // Step 4: Serialize tree
    std::string serializedTree = tree.serializeTree();
    int treeSize = serializedTree.size();

    std::cout << "DEBUG: Tree serialized, size: " << treeSize << " bytes" << std::endl;
    std::cout << "DEBUG: Tree preview (first 100 chars): ";
    for (int i = 0; i < std::min(100, treeSize); i++) {
        char c = serializedTree[i];
        if (c == '#') std::cout << "#";
        else if (c == '\0') std::cout << "\\0";
        else if (c < 32 || c > 126) std::cout << "?";
        else std::cout << c;
    }
    std::cout << std::endl;

    // Step 5: Open output file
    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout) {
        std::cerr << "ERROR: Cannot open output file: " << outputFile << std::endl;
        return false;
    }

    // Step 6: Write tree size as TEXT (to avoid endianness issues)
    std::string treeSizeStr = std::to_string(treeSize);
    fout.write(treeSizeStr.c_str(), treeSizeStr.size());
    fout.put('\n');  // Newline as delimiter

    std::cout << "DEBUG: Wrote tree size: " << treeSizeStr << std::endl;

    // Step 7: Write serialized tree
    fout.write(serializedTree.c_str(), treeSize);

    std::cout << "DEBUG: Wrote serialized tree" << std::endl;

    // Step 8: Get Huffman codes
    auto codes = tree.getCodes();
    std::cout << "DEBUG: Generated " << codes.size() << " Huffman codes" << std::endl;

    // Step 9: Encode and write compressed data
    unsigned char buffer = 0;
    int bitCount = 0;
    int totalBitsWritten = 0;

    for (unsigned char ch : inputData) {
        const std::string& code = codes[ch];

        for (char c : code) {
            // Add bit to buffer
            buffer = (buffer << 1) | (c == '1' ? 1 : 0);
            bitCount++;
            totalBitsWritten++;

            // If buffer is full (8 bits), write it
            if (bitCount == 8) {
                fout.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // Write any remaining bits (pad with zeros)
    if (bitCount > 0) {
        // Shift remaining bits to the left
        buffer <<= (8 - bitCount);
        fout.put(buffer);

        // Optional: You could store how many padding bits were added
        // For simplicity, we'll just pad with zeros
        std::cout << "DEBUG: Added " << (8 - bitCount) << " padding bits" << std::endl;
    }

    fout.close();

    std::cout << "DEBUG: Compression complete!" << std::endl;
    std::cout << "DEBUG: Total bits written: " << totalBitsWritten << std::endl;
    std::cout << "DEBUG: Original size: " << inputData.size() << " bytes" << std::endl;

    // Get the compressed file size
    std::ifstream finCheck(outputFile, std::ios::binary | std::ios::ate);
    if (finCheck) {
        std::streamsize compressedSize = finCheck.tellg();
        finCheck.close();
        std::cout << "DEBUG: Compressed size: " << compressedSize << " bytes" << std::endl;
        if (inputData.size() > 0) {
            double ratio = (double)inputData.size() * 8 / totalBitsWritten;
            std::cout << "DEBUG: Compression ratio: " << ratio << ":1" << std::endl;
        }
    }

    std::cout << "SUCCESS: Compression done! Output file: " << outputFile << std::endl;
    return true;
}
