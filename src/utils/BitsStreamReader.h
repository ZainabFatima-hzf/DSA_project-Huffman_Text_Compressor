#pragma once
#include <fstream>
#include <string>
#include <ios> // Necessary for std::streampos

class BitsStreamReader {
private:
    std::ifstream fin;
    unsigned char buffer;
    int bitCount;

public:
    // Declaration only (prototype)
    BitsStreamReader(const std::string& filename, std::streampos offset = 0);

    // Declaration only (prototype)
    int readBit();

    // Declaration only (prototype)
    bool isEOF();

    void skipBytes(int n);
    // Declaration only (prototype)
    ~BitsStreamReader();
};
