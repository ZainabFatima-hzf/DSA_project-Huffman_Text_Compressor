#pragma once
#include <fstream>
#include <string>

class BitsStreamReader {
private:
    std::ifstream fin;
    unsigned char buffer;
    int bitCount;

public:
    BitsStreamReader(const std::string& filename, std::streampos offset = 0) : buffer(0), bitCount(0) {
        fin.open(filename, std::ios::binary);
        fin.seekg(offset);
    }

    int readBit() {
        if (bitCount == 0) {
            fin.get((char&)buffer);
            if (fin.eof()) return -1;
            bitCount = 8;
        }
        int bit = (buffer >> 7) & 1;
        buffer <<= 1;
        bitCount--;
        return bit;
    }

    bool isEOF() { return fin.eof(); }

    ~BitsStreamReader() { fin.close(); }
};
