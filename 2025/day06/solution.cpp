#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <tuple>
#include <cctype>

bool parseCommandLine(int argc, char* argv[], std::string &filename) {
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--filename" && i + 1 < argc) {
            filename = argv[i + 1];
            return true;
        }
    }
    return false;
}

// Example reading function - adapt based on your input format
std::tuple<std::vector<std::vector<unsigned>>, std::vector<char>> readInputFile(const std::string &filename) {
    std::vector<std::vector<unsigned>> values;
    std::vector<char> symbols;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    std::vector<std::string> lines;
    
    // Read all lines
    while (std::getline(inFile, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    inFile.close();
    
    if (lines.empty()) {
        return std::make_tuple(values, symbols);
    }
    
    // Parse the last line as symbols
    std::string lastLine = lines.back();
    for (char ch : lastLine) {
        if (ch == '*' || ch == '+') {
            symbols.push_back(ch);
        }
    }
    
    // Parse all other lines as numbers
    for (size_t i = 0; i < lines.size() - 1; i++) {
        std::vector<unsigned> row;
        std::istringstream iss(lines[i]);
        unsigned num;
        
        while (iss >> num) {
            row.push_back(num);
        }
        
        if (!row.empty()) {
            values.push_back(row);
        }
    }
    
    return std::make_tuple(values, symbols);
}

int main(int argc, char* argv[]) {
    std::string filename;
    
    if (!parseCommandLine(argc, argv, filename)) {
        std::cerr << "Usage: " << argv[0] << " --filename <filename>" << std::endl;
        return 1;
    }
    
    try {
        auto [values, symbols] = readInputFile(filename);
        
        std::cout << "Read " << values.size() << " rows of numbers and " 
                  << symbols.size() << " symbols from file." << std::endl;
        
        // Your solution logic goes here
        size_t num = symbols.size();
        std::vector<unsigned long long> total;
        total.resize(num);

        unsigned long long grand_total = 0;

        for(unsigned j=0; j< num; j++) {
            if( symbols[j] == '*')
                    total[j] = 1;
                else if( symbols[j] == '+')
                    total[j] = 0;   

            for(size_t i=0; i< values.size(); i++) {
                if( symbols[j] == '*')
                    total[j] *= values[i][j];
                else if( symbols[j] == '+')
                    total[j] += values[i][j];                

            }

            grand_total += total[j];
        }
        
        std::cout << "Grand total: " << grand_total << std::endl;
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
