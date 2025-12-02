#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

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
    if (position < 0) {
        position += 100;
    }
    if (position >= 100) {
        position -= 100;
    }
}

void calculate_position_part2(int &position, bool bRight, int value, int &delta_part2) {
    int old_position = position;

    if (bRight) {
        position += value;
        delta_part2 += position / 100;
        position = position % 100;
    } else {
        // calculate how many times we hit 0
        if (value >= old_position && old_position > 0) {
            // We will cross 0: first hit at old_position clicks, then every 100
            delta_part2 += 1 + (value - old_position) / 100;
        } else if (old_position == 0 && value >= 100) {
            // Starting at 0: hit 0 every 100 clicks  
            delta_part2 += value / 100;
        }
        
        position -= value;
        position = ((position % 100) + 100) % 100;
    }
}

void calculate_position_part3(int &position, bool bRight, int value, int &delta_part2) {
    for(int i = 0; i < value; i++) {
        if (bRight) {
            position++;
            if (position == 100) {
                position = 0;
            }
        } else {
            position--;
            if (position < 0) {
                position = 99;
            }
        }
        
        // Count every time we land on position 0 during rotation
        if (position == 0) {
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

            std::cout << (dir.bRight ? "R" : "L") 
                      << std::setw(4) << dir.value 
                      << " | P1:" << std::setw(3) << start_position_p1
                      << " | P2:" << std::setw(3) << start_position_p2
                      << " | Delta:" << std::setw(5) << delta_part2 << std::endl;

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