#include "Decompressor.h"
#include <iostream>

int main() {
    Decompressor dec;
    dec.decompressFile("../test.huf", "../output.txt"); // adjust paths
    std::cout << "Decompression complete!" << std::endl;
    return 0;
}
