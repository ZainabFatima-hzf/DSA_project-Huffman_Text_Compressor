#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <vector>

class FileIO {
public:
    // Read an entire file into a vector of bytes
    static std::vector<unsigned char> readFile(const std::string& filename);

    // Write a vector of bytes into a file
    static void writeFile(const std::string& filename, const std::vector<unsigned char>& data);
};

#endif
