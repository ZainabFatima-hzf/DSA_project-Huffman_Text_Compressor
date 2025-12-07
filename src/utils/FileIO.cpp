#include "FileIO.h"
#include <fstream>
#include <iostream>

std::vector<unsigned char> FileIO::readFile(const std::string& filename) {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin) {
        std::cerr << "Error: Cannot open file " << filename << " for reading.\n";
        return {};
    }
    std::vector<unsigned char> data((std::istreambuf_iterator<char>(fin)),
                                     std::istreambuf_iterator<char>());
    fin.close();
    return data;
}

void FileIO::writeFile(const std::string& filename, const std::vector<unsigned char>& data) {
    std::ofstream fout(filename, std::ios::binary);
    if (!fout) {
        std::cerr << "Error: Cannot open file " << filename << " for writing.\n";
        return;
    }
    fout.write(reinterpret_cast<const char*>(data.data()), data.size());
    fout.close();
}
