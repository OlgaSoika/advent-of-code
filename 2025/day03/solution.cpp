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

std::vector<std::string> readInputFile(const std::string &filename) {
    std::vector<std::string> data;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) {
            data.push_back(line);
        }
    }

    inFile.close();
    return data;
}

unsigned long long find_number( const std::string& bank) {
    unsigned long long number = 0;
    int max_idx = 0;
    unsigned max = bank[0] - '0';

    for(int i = 1; i< bank.length() -1; i++) {
        unsigned num = bank[i] - '0';
        if( max < num) {
            max = num;
            max_idx = i;
        }
    }
    number = max*10;
    max_idx++;
    max = bank[max_idx] - '0';

    for(int i = max_idx+1; i< bank.length(); i++) {
        unsigned num = bank[i] - '0';
        if( max < num) {
            max = num;
            max_idx = i;
        }
    }

    return number + max;
}

unsigned long long find_number_part2( const int leave_digits, const std::string& bank) {
    unsigned long long number = 0;
    int max_idx = 0;
    unsigned max = bank[0] - '0';
    int num_idx = 1;

    for(int i = 1; i< bank.length() - leave_digits +1 ; i++) {
        unsigned num = bank[i] - '0';
        if( max < num) {
            max = num;
            max_idx = i;
        }
    }

    number = max;

    for (int k = num_idx; k < leave_digits; k++) {
        
        max_idx++;
        max = bank[max_idx] - '0';

        for(int i = max_idx+1; i< bank.length() - leave_digits + k + 1; i++) {
            unsigned num = bank[i] - '0';
            if( max < num) {
                max = num;
                max_idx = i;
            }
        }
        number = number*10 + max;
    }

    std::cout << "Part2 number: " << number << std::endl;

    return number;
}

int main(int argc, char* argv[]) {
    std::string filename;
    
    if (!parseCommandLine(argc, argv, filename)) {
        std::cerr << "Usage: " << argv[0] << " --filename <filename>" << std::endl;
        return 1;
    }
    
    try {
        std::vector<std::string> banks = readInputFile(filename);
        
        unsigned long long sum = 0;
        unsigned long long sum_part2 = 0;

        for (const auto &bank : banks) {
            std::cout << bank << std::endl;

            sum += find_number(bank);
            sum_part2 += find_number_part2(12, bank);
        }

        std::cout << "Read " << banks.size() << " banks from file." << std::endl;

        std::cout << "The sum of the maximum joltage from each bank: " << sum << std::endl;;
        std::cout << "The sum of the maximum joltage from each bank (part 2): " << sum_part2 << std::endl;;
        
        // Your solution logic goes here
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
