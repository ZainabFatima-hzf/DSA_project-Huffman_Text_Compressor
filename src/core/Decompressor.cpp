#include "Decompressor.h"
#include "../utils/BitsStreamReader.h"
#include "../utils/FileIO.h"
#include "HuffmanTree.h"
#include <fstream>
#include <iostream>

void Decompressor::decompressFile(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin) { std::cerr << "Cannot open input file: " << inputFile << std::endl; return; }

    // Read tree size
    int treeSize = 0;
    fin.read(reinterpret_cast<char*>(&treeSize), sizeof(treeSize));
    if (treeSize <= 0) { std::cerr << "Invalid file\n"; return; }

    // Read serialized tree
    std::string serializedTree(treeSize, '\0');
    fin.read(&serializedTree[0], treeSize);

    HuffmanTree tree;
    tree.deserializeTree(serializedTree);

    // Start reading compressed bits after tree
    BitsStreamReader reader(inputFile, sizeof(int) + treeSize);

    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout) { std::cerr << "Cannot open output file\n"; return; }

    HuffmanNode* current = tree.getRoot();
    int bit;
    while ((bit = reader.readBit()) != -1) {
        if (bit == 0) current = current->left;
        else current = current->right;

        if (!current->left && !current->right) { // leaf
            fout.put(current->data);
            current = tree.getRoot();
        }
    }

    fout.close();
    std::cout << "Decompression complete! Output file: " << outputFile << std::endl;
}
