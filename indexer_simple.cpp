// indexer_simple.cpp - File Indexer for EdgeOffice
// Compile: g++ -std=c++17 -O0 -o indexer_O0 indexer_simple.cpp
// Compile: g++ -std=c++17 -O2 -o indexer_O2 indexer_simple.cpp

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;


// Compute a simple hash (djb2 algorithm) of file contents
unsigned long simpleHash(const fs::path& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) return 0;

    unsigned long hash = 5381;
    char c;
    while (file.get(c)) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);  // hash * 33 + c
    }
    return hash;
}


// Convert unsigned long to 16-char hex string
std::string toHexString(unsigned long value) {
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << value;
    return ss.str();
}


// Index one file: get size, compute hash, write JSON record
void indexFile(const fs::path& filepath, std::ofstream& output) {
    try {
        auto filesize = fs::file_size(filepath);
        unsigned long hash = simpleHash(filepath);

        output << "{";
        output << "\"filename\":\"" << filepath.filename().string() << "\",";
        output << "\"path\":\"" << filepath.string() << "\",";
        output << "\"size\":" << filesize << ",";
        output << "\"hash\":\"" << toHexString(hash) << "\"";
        output << "}\n";

    } catch (const std::exception& e) {
        output << "{\"path\":\"" << filepath.string() << "\",\"error\":\"" << e.what() << "\"}\n";
    }
}


int main(int argc, char* argv[]) {
    // Check arguments
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <directory> [output_file]\n";
        return 1;
    }

    fs::path rootDir = argv[1];
    std::string outputFile = (argc >= 3) ? argv[2] : "index_results_cpp.jsonl";

    // Validate directory exists
    if (!fs::exists(rootDir)) {
        std::cerr << "Error: Directory not found: " << rootDir << "\n";
        return 1;
    }

    // Open output file
    std::ofstream output(outputFile);
    if (!output) {
        std::cerr << "Error: Cannot open output file\n";
        return 1;
    }

    std::cout << "Indexing: " << rootDir << "\n";

    // Start timing
    auto startTime = std::chrono::high_resolution_clock::now();
    int fileCount = 0;

    // Walk directory and index each file
    for (const auto& entry : fs::recursive_directory_iterator(rootDir)) {
        if (entry.is_regular_file()) {
            indexFile(entry.path(), output);
            fileCount++;
        }
    }

    // Stop timing and output results
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Indexed " << fileCount << " files in " << duration.count() / 1000.0 << " seconds\n";
    std::cout << "Results written to: " << outputFile << "\n";

    return 0;
}