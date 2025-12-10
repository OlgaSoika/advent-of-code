#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <set>

bool parseCommandLine(int argc, char* argv[], std::string &filename) {
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--filename" && i + 1 < argc) {
            filename = argv[i + 1];
            return true;
        }
    }
    return false;
}

std::vector<std::vector<char>> readInputFile(const std::string &filename) {
    std::vector<std::vector<char>> data;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) {
            std::vector<char> chars(line.begin(), line.end());
            data.push_back(chars);
        }
    }
    
    inFile.close();
    return data;
}

static int count{0};

std::set<std::pair<int, int>> beams_positions;

bool start(int row, int col, const std::vector<std::vector<char>> &data) {

    if ((row >= data.size()) || (col < 0) || (col >= data[0].size())) {
        return false;
    }

    std::pair<int, int> pos = {row, col};

    if (beams_positions.find(pos) != beams_positions.end()) {
        return false; // the current beam already exists
    }
    
    // save the beam coordinates in the set;
    beams_positions.insert({row, col});
    std::cout << "Beam at position (" << row << ", " << col << ") was set" << std::endl;

    return true;
}

void process(int row, int col, const std::vector<std::vector<char>> &data) {
    
    if ((row >= data.size()) || (col < 0) || (col >= data[0].size())) {
        return;
    }
    
    std::cout << "Position: (" << row << ", " << col << "), Value: '" << data[row][col] << "'" << std::endl;
    if (data[row][col] == '.') {
        // move down
        process(row+1, col, data);
        return;
    }

    if (data[row][col] == '^') {
        // split beam into left and right
        bool bSplitted = false;
        if (start(row, col - 1, data)) {
            // left beam
            process(row, col - 1, data);
            bSplitted = true;
        }
        if (start(row, col + 1, data)) {// right beam
            process(row, col + 1, data);
            bSplitted = true;
        }
        if (bSplitted) {
            count++;
        }
    } 
}

int main(int argc, char* argv[]) {
    std::string filename;
    
    if (!parseCommandLine(argc, argv, filename)) {
        std::cerr << "Usage: " << argv[0] << " --filename <filename>" << std::endl;
        return 1;
    }
    
    try {
        std::vector<std::vector<char>> data = readInputFile(filename);
        
        std::cout << "Read " << data.size() << " lines from file." << std::endl;
        
        //find symbol S

        size_t j = 0;
        for(; j< data[0].size(); j++) {
            if (data[0][j] == 'S') {
                break;
            }
        }
        
        // move down the rows and calculate beams
        start(1, j, data);
        process(1, j, data);

        std::cout << "Total beams: " << count << std::endl;
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
