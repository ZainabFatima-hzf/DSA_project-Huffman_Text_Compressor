#include "BitsStreamReader.h"
#include <fstream>
#include <string>

// =================================================================
// Fix 1: Corrected class name (BitsStreamReader::)
// =================================================================

// Constructor 1: Opens file and seeks to optional offset (aligned with standard header definition)
BitsStreamReader::BitsStreamReader(const std::string& filename, std::streampos offset)
    : fin(filename, std::ios::binary), buffer(0), bitCount(0) {
    if (fin.is_open() && offset > 0) {
        fin.seekg(offset);
    }
}

// NOTE: The second constructor (BitStreamReader(std::ifstream &inputStream...))
//       is not declared in your standard header, so it is commented out
//       to prevent further errors unless you declare it.
/*
BitsStreamReader::BitsStreamReader(std::ifstream &inputStream)
    : fin(std::move(inputStream)), buffer(0), bitCount(0) {}
*/


// skip n bytes from current position
void BitsStreamReader::skipBytes(int n) {
    // Access member 'fin'
    fin.seekg(n, std::ios::cur);
}

// read one bit
int BitsStreamReader::readBit() {
    // Access member 'bitCount'
    if(bitCount == 0) {
        char byte;
        // Access member 'fin'
        if(!fin.get(byte)) return -1; // EOF (returns -1 if stream fails or EOF)

        // Access member 'buffer'
        buffer = static_cast<unsigned char>(byte);

        // Access member 'bitCount'
        bitCount = 8;
    }

    // Access member 'buffer' and 'bitCount'
    int bit = (buffer >> (bitCount - 1)) & 1;
    bitCount--;
    return bit;
}

// Implementation for the declared destructor
BitsStreamReader::~BitsStreamReader() {
    if (fin.is_open()) {
        fin.close();
    }
}

// Implementation for the declared isEOF()
bool BitsStreamReader::isEOF() {
    return fin.eof();
}
