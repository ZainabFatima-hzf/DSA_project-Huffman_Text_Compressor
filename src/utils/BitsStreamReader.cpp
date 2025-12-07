#include "BitsStreamReader.h"

BitStreamReader::BitStreamReader(const std::string &filename) : fin(filename, std::ios::binary), buffer(0), bitCount(0) {}

BitStreamReader::BitStreamReader(std::ifstream &inputStream) : fin(std::move(inputStream)), buffer(0), bitCount(0), externalStream(true) {}

// skip n bytes from current position
void BitStreamReader::skipBytes(int n) {
    fin.seekg(n, std::ios::cur);
}

// read one bit
int BitStreamReader::readBit() {
    if(bitCount == 0) {
        char byte;
        if(!fin.get(byte)) return -1; // EOF
        buffer = static_cast<unsigned char>(byte);
        bitCount = 8;
    }

    int bit = (buffer >> (bitCount - 1)) & 1;
    bitCount--;
    return bit;
}
