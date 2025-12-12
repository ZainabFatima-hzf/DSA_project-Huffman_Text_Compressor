#pragma once
#include <fstream>
#include <string>

class BitsStreamWriter {
private:
    std::ofstream fout;
    unsigned char buffer;
    int bitCount;

public:
    // Constructor declaration
    // The implementation (fout.open, buffer = 0, etc.) must be in the .cpp file.
    BitsStreamWriter(const std::string& filename);

    // Method declarations (prototypes)
    void writeBit(int bit);
    void writeBits(const std::string& bits);
    void flush();

    // Destructor declaration
    // The implementation (~BitsStreamWriter() { flush(); fout.close(); })
    // must be in the .cpp file.
    ~BitsStreamWriter();
};
