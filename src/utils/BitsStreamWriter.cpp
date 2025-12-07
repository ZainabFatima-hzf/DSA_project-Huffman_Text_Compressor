#include "BitsStreamWriter.h"
#include <iostream>



BitStreamWriter::BitStreamWriter(const std::string &filename) {
    fout.open(filename, std::ios::binary);
    if(!fout.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
    }
    buffer = 0;
    bitCount = 0;
}

void BitStreamWriter::writeBit(int bit) {
    buffer <<= 1;
    buffer |= (bit & 1); // set the last bit
    bitCount++;

    if(bitCount == 8) {
        fout.put(buffer); // write full byte
        buffer = 0;
        bitCount = 0;
    }
}

void BitStreamWriter::writeBits(const std::string &bits) {
    for(char b : bits) {
        writeBit(b - '0'); // convert '0'/'1' char to int
    }
}

void BitStreamWriter::flush() {
    if(bitCount > 0) {
        buffer <<= (8 - bitCount); // pad remaining bits with 0
        fout.put(buffer);
        buffer = 0;
        bitCount = 0;
    }
}

BitStreamWriter::~BitStreamWriter() {
    flush();       // flush any remaining bits
    fout.close();  // close the file
}
/*int main() {
    BitStreamWriter writer("../test.huf"); // output in src/

    writer.writeBits("101");   // write 3 bits
    writer.writeBits("1110");  // write 4 more bits
    writer.flush();            // flush remaining bits

    return 0;
}*/