#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Direction {
    bool bRight;  // true for R, false for L
    int value;    // 0-99
};

bool parseCommandLine(int argc, char* argv[], std::string &filename) {
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--filename" && i + 1 < argc) {
            filename = argv[i + 1];
            return true;
        }
    }
    return false;
}

std::vector<Direction> readDirections(const std::string &filename) {
    std::vector<Direction> directions;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }
        
        Direction dir;
        
        // Parse first character (L or R)
        if (line[0] == 'R') {
            dir.bRight = true;
        } else if (line[0] == 'L') {
            dir.bRight = false;
        } else {
            std::cerr << "Invalid direction: " << line << std::endl;
            continue;
        }
        
        // Parse the number
        try {
            dir.value = std::stoi(line.substr(1));
            
            directions.push_back(dir);
        } catch (const std::exception &e) {
            std::cerr << "Error parsing number in line: " << line << std::endl;
            continue;
        }
    }
    
    inFile.close();
    return directions;
}

void calculate_position_part1(int &position, bool bRight, int value) {
    if (bRight) {
        position += value % 100;
    } else {
        position -= value % 100;
    }
    
    // Wrap around logic for positions 0-99
    while (position < 0) {
        position += 100;
    }
    while (position >= 100) {
        position -= 100;
    }
}

void calculate_position_part2(int &position, bool bRight, int value, int &delta_part2) {
    if (bRight) {
        position += value;
        
        while (position >= 100) {
            position -= 100;
            delta_part2++;
        }
    } else {
        position -= value;
        while (position < 0) {
            position += 100;
            delta_part2++;
        }
    }
}
int main(int argc, char* argv[]) {
    std::string filename;
    
    if (!parseCommandLine(argc, argv, filename)) {
        std::cerr << "Usage: " << argv[0] << " --filename <filename>" << std::endl;
        return 1;
    }
    
    int start_position_p1 = 50; // starting position
    int zero_positions = 0;

    int start_position_p2 = 50; // starting position
    int delta_part2 = 0;    // extra positions for the part2 when dial points at 0
    int zero_positions_p2 = 0;
        
    try {
        std::vector<Direction> directions = readDirections(filename);
        
        // Print parsed directions for verification
        std::cout << "Parsed " << directions.size() << " directions:" << std::endl;

   
        for (const auto &dir : directions) {

            calculate_position_part1( start_position_p1, dir.bRight, dir.value );
            calculate_position_part2( start_position_p2, dir.bRight, dir.value, delta_part2 );

            std::cout << "Direction: " << (dir.bRight ? "R" : "L") 
                      << ", Value: " << dir.value 
                      << ", Position P1: " << start_position_p1
                      << ", Position P2: " << start_position_p2
                      << ", Delta Part 2: " << delta_part2 << std::endl;

            if(start_position_p1 == 0) {
                zero_positions++;
            }

            if(start_position_p2 == 0) {
                zero_positions_p2++;
            }
        }
        
        // Your solution logic goes here
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "The actual password to open the door (part 1): " << 
        zero_positions << std::endl;
    std::cout << "The actual password to open the door (part 2): " << 
        delta_part2 << std::endl;

    return 0;
}