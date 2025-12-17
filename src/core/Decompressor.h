#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include <string>
#include "../utils/BitsStreamReader.h"
#include "HuffmanTree.h"
//class decompressor
class Decompressor {
public:
    bool decompressFile(const std::string &inputFile, const std::string &outputFile);
};

#endif
