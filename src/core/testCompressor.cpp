#include "Compressor.h"
#include <iostream>

int main() {
    Compressor compressor;
    std::string inputFile = "../../sample.txt";
    std::string outputFile = "../../sample.huf";

    compressor.compressFile(inputFile, outputFile);

    std::cout << "Compression done!" << std::endl;
    return 0;
}
