#include "Decompressor.h"
#include "../utils/FileIO.h"
#include "HuffmanTree.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>

bool Decompressor::decompressFile(const std::string &inputFile, const std::string &outputFile) {
    std::cout << "DEBUG: Starting decompression of " << inputFile << std::endl;

    // Step 1: Open input file
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin) {
        std::cerr << "ERROR: Cannot open input file: " << inputFile << std::endl;
        return false;
    }

    // Get file size
    fin.seekg(0, std::ios::end);
    size_t fileSize = fin.tellg();
    fin.seekg(0, std::ios::beg);

    std::cout << "DEBUG: File size: " << fileSize << " bytes" << std::endl;

    if (fileSize == 0) {
        std::cerr << "ERROR: File is empty" << std::endl;
        return false;
    }

    // Step 2: Read tree size as TEXT (until newline)
    std::string treeSizeStr;
    char ch;

    while (fin.get(ch) && ch != '\n') {
        treeSizeStr += ch;

        // Safety check: tree size shouldn't be too long
        if (treeSizeStr.size() > 20) {
            std::cerr << "ERROR: Tree size string too long - invalid file format" << std::endl;
            return false;
        }
    }

    // Check if we found a newline
    if (fin.eof()) {
        std::cerr << "ERROR: No newline found after tree size - invalid file format" << std::endl;
        return false;
    }

    std::cout << "DEBUG: Tree size string: '" << treeSizeStr << "'" << std::endl;

    // Convert to integer
    int treeSize;
    try {
        treeSize = std::stoi(treeSizeStr);
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Invalid tree size format: " << treeSizeStr << std::endl;
        return false;
    }

    std::cout << "DEBUG: Tree size: " << treeSize << " bytes" << std::endl;

    // Validate tree size
    if (treeSize <= 0) {
        std::cerr << "ERROR: Tree size must be positive, got: " << treeSize << std::endl;
        return false;
    }

    if (treeSize > 1000000) { // Reasonable limit: 1MB for tree
        std::cerr << "ERROR: Tree size too large: " << treeSize << " bytes" << std::endl;
        return false;
    }

    // Check if file has enough data
    std::streampos currentPos = fin.tellg();
    if (fileSize < currentPos + treeSize) {
        std::cerr << "ERROR: File too small for tree data. Need " << treeSize
                  << " bytes, have " << (fileSize - currentPos) << " bytes" << std::endl;
        return false;
    }

    // Step 3: Read serialized tree
    std::string serializedTree(treeSize, '\0');
    fin.read(&serializedTree[0], treeSize);

    std::streamsize bytesRead = fin.gcount();
    if (bytesRead != treeSize) {
        std::cerr << "ERROR: Failed to read full tree. Expected "
                  << treeSize << " bytes, got " << bytesRead << " bytes" << std::endl;
        return false;
    }

    std::cout << "DEBUG: Tree data preview (first 100 chars): ";
    for (int i = 0; i < std::min(100, treeSize); i++) {
        char c = serializedTree[i];
        if (c == '#') std::cout << "#";
        else if (c == '\0') std::cout << "\\0";
        else if (c < 32 || c > 126) std::cout << "?";
        else std::cout << c;
    }
    std::cout << std::endl;

    // Step 4: Deserialize tree
    HuffmanTree tree;
    std::cout << "DEBUG: Deserializing tree..." << std::endl;
    tree.deserializeTree(serializedTree);

    HuffmanNode* root = tree.getRoot();
    if (root == nullptr) {
        std::cerr << "ERROR: Failed to deserialize Huffman tree (root is null)" << std::endl;
        return false;
    }

    std::cout << "DEBUG: Tree deserialized successfully" << std::endl;

    // Step 5: Open output file
    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout) {
        std::cerr << "ERROR: Cannot open output file: " << outputFile << std::endl;
        return false;
    }

    // Step 6: Decode compressed data
    HuffmanNode* current = root;
    unsigned char byte;
    int bitsProcessed = 0;
    int bytesWritten = 0;

    std::cout << "DEBUG: Starting decompression from position: " << fin.tellg() << std::endl;

    while (fin.get(reinterpret_cast<char&>(byte))) {
        // Process all 8 bits in this byte
        for (int bitPos = 7; bitPos >= 0; bitPos--) {
            bitsProcessed++;
            int bit = (byte >> bitPos) & 1;

            // Traverse tree
            if (bit == 0) {
                current = current->left;
            } else {
                current = current->right;
            }

            // Check for null pointer
            if (!current) {
                std::cerr << "ERROR: Null node encountered at bit " << bitsProcessed
                          << " (byte value: " << (int)byte << ", bit position: " << bitPos << ")" << std::endl;
                return false;
            }

            // Check if we reached a leaf node
            if (!current->left && !current->right) {
                fout.put(current->character);
                bytesWritten++;
                current = root; // Reset to root
            }
        }
    }

    fout.close();
    fin.close();

    std::cout << "DEBUG: Decompression statistics:" << std::endl;
    std::cout << "DEBUG:   Total bits processed: " << bitsProcessed << std::endl;
    std::cout << "DEBUG:   Bytes written: " << bytesWritten << std::endl;

    if (current != root) {
        std::cout << "WARNING: Ended decompression not at root (some padding bits at the end)" << std::endl;
    }

    std::cout << "SUCCESS: Decompression complete! Output file: " << outputFile << std::endl;
    return true;
}
