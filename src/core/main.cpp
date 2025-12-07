#include "../utils/FileIO.h"
#include <iostream>

int main() {
    // Read file
    auto data = FileIO::readFile("src/core/test.txt");
    std::cout << "Read " << data.size() << " bytes.\n";

    // Write to a new file
    FileIO::writeFile("test_copy.txt", data);
    std::cout << "File written as test_copy.txt\n";

    return 0;
}
