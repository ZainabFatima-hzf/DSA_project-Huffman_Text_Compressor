# Huffman Text Compressor (C++)

## Overview
This project implements a **Huffman Text Compressor and Decompressor** in C++. It compresses text files into `.huf` format using the Huffman coding algorithm and can decompress them back to the original text. The system demonstrates **file I/O, bit-level operations, and tree-based encoding**, making it a complete example of data compression algorithms.


## Features
- Compress any text file to a `.huf` file using Huffman coding.
- Decompress `.huf` files back to the original text.
- Efficient **BitStreamWriter/BitStreamReader** for bit-level operations.
- Handles **empty files** and files with repeated characters.
- Clean, modular, and object-oriented C++ code structure.
- Easy to extend for UTF-8 support and non-ASCII characters.

---

## Folder Structure

DSA_project-Huffman_Text_Compressor/
│
├─ core/
│ ├─ Compressor.cpp
│ ├─ Decompressor.cpp
│ ├─ HuffmanTree.cpp
│ ├─ testCompressor.cpp
│ └─ testDecompressor.cpp
│
├─ utils/
│ ├─ FileIO.cpp
│ ├─ FileIO.h
│ ├─ BitsStreamWriter.cpp
│ ├─ BitsStreamWriter.h
│ ├─ BitsStreamReader.cpp
│ └─ BitsStreamReader.h
│
├─ models/
│ └─ HuffmanNode.cpp / HuffmanNode.h
│
├─ structures/
│ └─ MinHeap.cpp / MinHeap.h
│
├─ sample.txt ← Example input file
├─ sample.huf ← Example compressed file (output)
└─ README.md

Usage
Compression:
./testCompressor.exe sample.txt sample.huf
Output: sample.huf (compressed file)
Decompression:
./testDecompressor.exe sample.huf output.txt

Testing & Validation
Test cases to verify correctness:
1.Small text files (sample.txt)
2.Empty files
3.Files with single repeated characters
4.Compression → Decompression → Verify output equals original
5.Check file size reduction (compression ratio)

Algorithm Overview
1.Frequency Counter: Count occurrence of each character in input file.
2.Huffman Tree: Build tree based on character frequencies.
3.BitsStreamWriter: Write bits efficiently to .huf file.
4.Serialization: Save Huffman tree structure in .huf for decompression.
5.Decompression: Rebuild tree, read bits, and decode characters.

Dependencies
1.Standard C++17 libraries (<fstream>, <iostream>, <unordered_map>, <queue>)
2.No external libraries required.

Future Improvements
1.Add UTF-8 support for non-ASCII characters.
2.Implement GUI or console progress bar for large files.
3.Optimize memory usage in BitsStreamReader and BitsStreamWriter.
4.Exception handling for invalid/corrupted .huf files.


GitHub Workflow

1.Initialize Git in project root:
git init
2.Add files and commit:
git add .
git commit -m "Initial commit: Huffman Compressor project"
3.Connect to remote repository:
git branch -M main
git remote add origin <your-github-repo-url>
git push -u origin main