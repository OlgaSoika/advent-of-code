#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

int is_accessible(int row, int col, const std::vector<std::vector<char>> &data) {
    // Implement your logic here to determine if the position is accessible
    // For example, check if the character at data[row][col] is not a wall

    constexpr int max_num = 4;
    int idx_max = data.size()-1;
    if( (!row && !col) || (!row && (col==idx_max)) || ((row==idx_max) && !col) || ((row==idx_max) && (col==idx_max)))
        return true;

    int num = 0;
    
    if(row) {
        num += data[row-1][col] == '@' ? 1 : 0;
        if(col)
            num += data[row-1][col-1] == '@' ? 1 : 0;
        if(col!=idx_max)
            num += data[row-1][col+1] == '@' ? 1 : 0;
    }

    if(row!= idx_max) {
        num += data[row+1][col] == '@' ? 1 : 0;
        if(col)
            num += data[row+1][col-1] == '@' ? 1 : 0;
        if(col!=idx_max)
            num += data[row+1][col+1] == '@' ? 1 : 0;
    }
    
    if(col) 
        num += data[row][col-1] == '@' ? 1 : 0;
    if(col!=idx_max)
        num += data[row][col+1] == '@' ? 1 : 0;

    return num < max_num;
}

int calc_rolls(const std::vector<std::vector<char>> &data) {
    // Implement your logic here
    int num = 0;
    for(int i = 0; i<data.size(); i++) {
        for(int j = 0; j < data[i].size(); j++) {
            // Process each character
            if(data[i][j] != '@') 
                continue;
            if (is_accessible(i, j, data)) {
                num++;
            }
        }
    }
    return num;
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
        
        // Your solution logic goes here

        int num_rolls = calc_rolls(data);
        std::cout << "Number of rolls: " << num_rolls << std::endl;
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
