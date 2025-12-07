#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <tuple>

bool parseCommandLine(int argc, char* argv[], std::string &filename) {
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--filename" && i + 1 < argc) {
            filename = argv[i + 1];
            return true;
        }
    }
    return false;
}

std::tuple<std::vector<std::pair<unsigned long long, unsigned long long>>, std::vector<unsigned long long>> readInputFile(const std::string &filename) {
    std::vector<std::pair<unsigned long long, unsigned long long>> ranges;
    std::vector<unsigned long long> ingredients;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    bool readingRanges = true;
    
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            // Empty line marks the transition from ranges to ingredients
            readingRanges = false;
            continue;
        }
        
        if (readingRanges) {
            // Parse range in format "3-5"
            size_t dashPos = line.find('-');
            if (dashPos != std::string::npos) {
                unsigned long long first = std::stoull(line.substr(0, dashPos));
                unsigned long long second = std::stoull(line.substr(dashPos + 1));
                ranges.push_back({first, second});
            }
        } else {
            // Parse single ingredient value
            ingredients.push_back(std::stoull(line));
        }
    }
    
    inFile.close();
    return std::make_tuple(ranges, ingredients);
}

int main(int argc, char* argv[]) {
    std::string filename;
    
    if (!parseCommandLine(argc, argv, filename)) {
        std::cerr << "Usage: " << argv[0] << " --filename <filename>" << std::endl;
        return 1;
    }
    
    try {
        auto [ranges, ingredients] = readInputFile(filename);
        
        std::cout << "Read " << ranges.size() << " ranges and " 
                  << ingredients.size() << " ingredients from file." << std::endl;
        
        // Your solution logic goes here

        unsigned long long count = 0;
        for(auto id: ingredients) {
            bool bValid = false;

            for(auto range = ranges.begin(); !bValid && (range != ranges.end()); range++ ) {
                if(id >= range->first && id <= range->second) {
                    count++;
                    bValid = true;
                }
            }
        }

        std::cout << "The number of valid ingredients: " << count << std::endl;
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
