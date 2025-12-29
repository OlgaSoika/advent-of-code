#include <algorithm>
#include <bitset>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>

#include <string>
#include <vector>

constexpr size_t TILES_PER_BYTE = 8;

constexpr size_t TILES_PER_BLOCK = sizeof(uint64_t) * TILES_PER_BYTE;

constexpr size_t BYTES_PER_BLOCK = sizeof(uint64_t);

size_t TILES_SIZE = 0;

size_t TILES_PER_ITEM = 0;

bool parseCommandLine(int argc, char* argv[], std::string &filename) {
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--filename" && i + 1 < argc) {
            filename = argv[i + 1];
            return true;
        }
    }
    return false;
}

std::vector<std::pair<unsigned long long, unsigned long long>> readInputFile(const std::string &filename) {
    std::vector<std::pair<unsigned long long, unsigned long long>> data;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) {
            std::stringstream ss(line);
            unsigned long long x, y;
            char comma;
            
            if (ss >> x >> comma >> y) {
                data.push_back(std::make_pair(x, y));
            }
        }
    }

    inFile.close();
    return data;
}

bool is_red_and_green(const std::vector<std::vector<char>> &tiles, 
                      unsigned long long x1, unsigned long long y1,
                      unsigned long long x2, unsigned long long y2) {
    
    std::cout << "Checking rectangle (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")" << std::endl;
    for(unsigned long long x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
        for(unsigned long long y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            if (tiles[y][x] == '.') {
                std::cout << "  Found '.' at (" << x << "," << y << ") -> FALSE" << std::endl;
                return false;
            }
        }
    }
    std::cout << "  All 'A' -> TRUE" << std::endl;
    return true;
}
/*
void mark_tile(std::vector<std::vector<char>> &tiles, 
               unsigned long long x1, unsigned long long y1,
               unsigned long long x2, unsigned long long y2) {
    
            

}*/

void mark_area(std::vector<std::vector<char>> &tiles, 
               unsigned long long x1, unsigned long long y1,
               unsigned long long x2, unsigned long long y2) {
    std::cout << "Marking line from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")" << std::endl;
    if(x1 == x2) {
        // vertical line
        for (size_t y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
            tiles[y][x1] = 'A';
        }
    } else if (y1 == y2) {
        // horizontal line
        for (size_t x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
            tiles[y1][x] = 'A';
        }
    } else {
        std::cout << "Error: non straight line from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")" 
            << std::endl;
        throw std::runtime_error("Only horizontal or vertical lines are supported.");
    }
        
}   

int main(int argc, char* argv[]) {
    std::string filename;
    
    if (!parseCommandLine(argc, argv, filename)) {
        std::cerr << "Usage: " << argv[0] << " --filename <filename>" << std::endl;
        return 1;
    }
    
    try {
        std::vector<std::pair<unsigned long long, unsigned long long>> data = readInputFile(filename);
        
        std::cout << "Read " << data.size() << " lines from file." << std::endl;
        
        // part1 solution
        unsigned long long max_area = 0;
        unsigned long long max_x = data[0].first;
        unsigned long long max_y = data[0].second;
        
        for (size_t i = 0; i < data.size(); i++) {
            auto [x1, y1] = data[i];

            if (x1 > max_x) {
                max_x = x1;
            }
            if (y1 > max_y) {
                max_y = y1;    
            }

            for (size_t j = i+1; j < data.size(); j++) {
                auto [x2, y2] = data[j];
                
                unsigned long long area = (std::abs((long long)(x1 - x2))+1) * (std::abs((long long)(y1 - y2))+1);
                
                if(area > max_area) {
                    max_area = area;
                }
            }
        }

        // part2 solution
        TILES_SIZE = std::max(max_x, max_y) + 1;
        const size_t BLOCKS_SIZE = TILES_SIZE; //  / TILES_PER_BLOCK + ((TILES_SIZE % TILES_PER_BLOCK) ? 1 : 0);
        //const size_t BYTES_SIZE = ITEMS_SIZE * BYTES_PER_BLOCK;

        using tiles_block = std::bitset<TILES_PER_BLOCK>;

        std::vector<std::vector<char>> tiles(BLOCKS_SIZE, std::vector<char>(BLOCKS_SIZE, '.'));

        auto [x1, y1] = data[0];
        for (size_t i = 1; i < data.size(); i++) {    
            auto [x2, y2] = data[i];
    
            mark_area(tiles, x1, y1, x2, y2);

            std::tie(x1, y1) = std::make_pair(x2, y2);
        }
        //for the first point and the last point
        std::tie(x1, y1) = data[0];
        auto [x2, y2] = data[data.size() - 1];
        std::cout << "DEBUG: Closing loop from (" << x1 << "," << y1 << ") to (" << x2 << "," << y2 << ")" << std::endl;
    
        mark_area(tiles, x1, y1, x2, y2);
            
        unsigned long long max_area_part_2 = 0;
        for (size_t y = 0; y < BLOCKS_SIZE; y++) {
            //now find X of 'A's in the row

            auto it = std::find(tiles[y].begin(), tiles[y].end(), 'A');
            if(it != tiles[y].end()) {
                size_t first_x = std::distance(tiles[y].begin(), it);
                
                auto rit = std::find(tiles[y].rbegin(), tiles[y].rend(), 'A');
                size_t last_x = BLOCKS_SIZE - 1 - std::distance(tiles[y].rbegin(), rit);
                
                for(size_t x = first_x; x<=last_x; x++) {
                    //mark the area between first_x and last_x as 'A'
                    tiles[y][x] = 'A';
                }

//                size_t area = (last_x - first_x + 1);
//                std::cout << "Row " << y << ": first_x=" << first_x << ", last_x=" << last_x 
//                          << ", area=" << area << ", total=" << (max_area_part_2 + area) << std::endl;
//                max_area_part_2 += area;
            }   
        }

        for (size_t i = 0; i < data.size(); i++) {
            auto [x1, y1] = data[i];

            for (size_t j = i+1; j < data.size(); j++) {
                auto [x2, y2] = data[j];
                
                if(!is_red_and_green(tiles, x1, y1, x2, y2)) {
                    continue; // both points must be on the shape
                }
                unsigned long long area = (std::abs((long long)(x1 - x2))+1) * (std::abs((long long)(y1 - y2))+1);
                
                if(area > max_area_part_2) {
                    max_area_part_2 = area;
                }
            }
        }

        // Output tiles grid
        size_t out_size = (BLOCKS_SIZE > 60 ? 60 : BLOCKS_SIZE);
        for (size_t y = 0; y < out_size; y++) {
            for (size_t x = 0; x < out_size; x++) {
                std::cout << tiles[y][x];
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << "Maximum area for the part 1: " << max_area << std::endl;
        std::cout << "Total area for the part 2: " << max_area_part_2 << std::endl;
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
