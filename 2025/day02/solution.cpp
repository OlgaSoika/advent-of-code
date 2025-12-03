#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

bool parseCommandLine(int argc, char* argv[], std::string &filename) {
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--filename" && i + 1 < argc) {
            filename = argv[i + 1];
            return true;
        }
    }
    return false;
}

std::vector<std::pair<std::string, std::string>> readFile(const std::string &filename) {
    std::vector<std::pair<std::string, std::string>> pairs;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string token;
        
        // Parse comma-separated tokens
        while (std::getline(ss, token, ',')) {
            // Skip empty tokens
            if (token.empty()) {
                continue;
            }
            
            // Find the dash separator
            size_t dashPos = token.find('-');
            if (dashPos != std::string::npos) {
                std::string first = token.substr(0, dashPos);
                std::string second = token.substr(dashPos + 1);
                pairs.push_back({first, second});
            }
        }
    }
    
    return pairs;
}

unsigned long long calculate_sum(const std::string &first, const std::string &second) {
    unsigned long long sum = 0;

    unsigned long long start_val = std::stoull(first);
    unsigned long long end_val = std::stoull(second);

    for(unsigned long long val = start_val; val <= end_val; val++ ) {
        std::string val_str = std::to_string(val);

        size_t len = val_str.length();

        if( len%2 )
            continue;
        
        if( !val_str.compare(0, len/2, val_str, len/2, len/2)) {
            sum += val;
            std::cout << "Part 1 invalid value: " << val << std::endl;
        }
    
    }

    return sum;
}

bool is_invalid_value(std::string str_value, size_t part_len) {
    // the function checks if the string is made of identical parts
    // each part has length of part_len

    if( str_value.length() % part_len != 0 )
        return false;

    for(size_t i = 1; i < str_value.length()/part_len; i++) {
        if( str_value.compare(0, part_len, str_value, i*part_len, part_len) != 0 )
            return false;
    }
    return true;
}

unsigned long long calculate_sum_part2(const std::string &first, const std::string &second) {
    unsigned long long sum = 0;

    unsigned long long start_val = std::stoull(first);
    unsigned long long end_val = std::stoull(second);

    for(unsigned long long val = start_val; val <= end_val; val++ ) {
        std::string val_str = std::to_string(val);

        size_t digits = val_str.length();

        bool bFound = false;
        for(size_t len = 1; !bFound && (len <= digits/2); len++) {
        
            if( is_invalid_value(val_str, len)) {
                sum += val;
                std::cout << "Part2 invalid value: " << val << std::endl;
                bFound = true;
            }
        }
    
    }

    return sum;
}

int main(int argc, char* argv[]) {
    std::string filename;
    
    if (!parseCommandLine(argc, argv, filename)) {
        std::cerr << "Usage: " << argv[0] << " --filename <filename>" << std::endl;
        return 1;
    }
    
    try {
        std::vector<std::pair<std::string, std::string>> pairs = readFile(filename);
        
        unsigned long long sum = 0;
        unsigned long long sum_part2 = 0;

        for (const auto &p : pairs) {
            std::cout << p.first << " - " << p.second << std::endl;

            sum += calculate_sum(p.first, p.second);
            sum_part2 += calculate_sum_part2(p.first, p.second);

        }

        std::cout << "Read " << pairs.size() << " pairs from file." << std::endl;

        std::cout << "Sum of all invalid IDs:" << sum << std::endl;
        std::cout << "Sum of all invalid IDs, new rule (part 2):" << sum_part2 << std::endl;
        
        // Your solution logic goes here
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
