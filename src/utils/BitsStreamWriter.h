#pragma once
#include <fstream>
#include <string>

class BitsStreamWriter {
private:
    std::ofstream fout;
    unsigned char buffer;
    int bitCount;

public:
    BitsStreamWriter(const std::string& filename) : buffer(0), bitCount(0) {
        fout.open(filename, std::ios::binary | std::ios::app);
    }

    void writeBit(int bit) {
        buffer <<= 1;
        buffer |= (bit & 1);
        bitCount++;
        if (bitCount == 8) flush();
    }

    void writeBits(const std::string& bits) {
        for (char c : bits) writeBit(c - '0');
    }

    void flush() {
        if (bitCount == 0) return;
        buffer <<= (8 - bitCount);
        fout.put(buffer);
        buffer = 0;
        bitCount = 0;
    }

    ~BitsStreamWriter() { flush(); fout.close(); }
};
