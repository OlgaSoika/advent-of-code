#include <bitset>
#include <cstdint>
#include <stdexcept>

#include <iostream>
#include <fstream>

#include <set>
#include <string>

#include <tuple>
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

using buttons_type = std::vector<std::set<u_int16_t>>;
//masine launch bitset, buttons sets.
using data_type = std::tuple<std::bitset<16>, buttons_type>;


using bitset_vector_type = std::vector<std::bitset<16>>;
//masine launch bitset, vector of buttons bitsets.
using diagram_type = std::tuple<std::bitset<16>, bitset_vector_type>;

constexpr int LAUNCH_BITSET = 0;
constexpr int BUTTONS = 1;

std::vector<data_type> readInputFile(const std::string &filename) {
    std::vector<data_type> data;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    u_int16_t num_bits = 0;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        
        // Parse the bitset from [...]
        size_t openBracket = line.find('[');
        size_t closeBracket = line.find(']');
        std::string pattern = line.substr(openBracket + 1, closeBracket - openBracket - 1);
                if (pattern.length() > 16) {
            throw std::runtime_error("Pattern length " + std::to_string(pattern.length()) + " exceeds maximum bitset size of 16");
        }
        
        std::bitset<16> bits;
        for (size_t i = 0; i < pattern.length(); i++) {
            if (pattern[i] == '#') {
                bits[i] = 1;
            }
            num_bits++;
        }
        
        // Parse all numbers from (...)
        buttons_type buttons;
        size_t pos = closeBracket;
        while ((pos = line.find('(', pos)) != std::string::npos) {
            size_t closePos = line.find(')', pos);
            std::string content = line.substr(pos + 1, closePos - pos - 1);
            std::set<u_int16_t> buttonSet;
            
            // Parse comma-separated numbers
            size_t numPos = 0;
            while (numPos < content.length()) {
                size_t commaPos = content.find(',', numPos);
                if (commaPos == std::string::npos) {
                    commaPos = content.length();
                }
                std::string numStr = content.substr(numPos, commaPos - numPos);
                // Trim whitespace
                numStr.erase(0, numStr.find_first_not_of(" \t"));
                numStr.erase(numStr.find_last_not_of(" \t") + 1);
                
                if (!numStr.empty()) {
                    buttonSet.insert(static_cast<u_int16_t>(std::stoi(numStr)));
                }
                numPos = commaPos + 1;
            }
            
            buttons.push_back(buttonSet);
            pos = closePos + 1;
        }

        data.push_back(std::make_tuple(bits, buttons));
    }

    inFile.close();
    return data;
}

int main(int argc, char* argv[]) {
    std::string filename;
    
    if (!parseCommandLine(argc, argv, filename)) {
        std::cerr << "Usage: " << argv[0] << " --filename <filename>" << std::endl;
        return 1;
    }
    
    try {

        std::vector<data_type> data = readInputFile(filename);
        
        std::cout << "Read " << data.size() << " lines from file." << std::endl;
        
        // Your solution logic goes here
        //restructure input data to a vector of bitmaps and button sets

        std::vector<diagram_type> mashines;

        for(auto &entry : data) {
            bitset_vector_type bits_vector;
            const auto &bits = std::get<LAUNCH_BITSET>(entry);
            const auto &buttons = std::get<BUTTONS>(entry);
            
            std::cout << "Bits: " << bits << " Buttons: { ";
            for (const auto &buttonSet : buttons) {
                std::bitset<16> btn_bits;
                std::cout << "( ";
                for (const auto &btn : buttonSet) {
                    std::cout << btn << " ";
                    btn_bits.set(btn);
                }
                std::cout << ") ";
                bits_vector.push_back(btn_bits);
            }
            std::cout << "}" << std::endl;

            mashines.push_back(std::make_tuple(bits, bits_vector));
        }
        
        // find miniml number of items;
        std::cout << "Calculating minimum number of buttons..." << std::endl;
        int sum = 0;
        for (const auto &mashine : mashines) {
            const auto &buttons = std::get<BUTTONS>(mashine);
            size_t num_buttons = buttons.size();
            int min = INT32_MAX;
            //go throw all combinations make  XOR and then check

            std::cout << "Evaluating mashine with launch bitset " << std::get<LAUNCH_BITSET>(mashine) << " and " << num_buttons << " buttons." << std::endl;
            std::cout << "Num buttons " << num_buttons << std::endl;
            for(size_t i = 1; i < (1 << num_buttons); i++) {
                std::bitset<16> combined;
                int count_bits = 0;
                for(size_t j = 0; j < num_buttons; j++) {
                    if(i & (1 << j)) { //checks if j-th bit is set in i
                        combined ^= buttons[j]; // in that case include button j
                        count_bits++;
                    }
                }
                
                if(combined == std::get<LAUNCH_BITSET>(mashine)) {
                    min = std::min(min, count_bits);
                    std::cout << "Found combination: " << std::bitset<16>(i) << " with " << count_bits << " buttons." << std::endl; 
                    //this is similar to above and more effective:
                    //if (__builtin_popcount(i) < min) {
                    //    min = __builtin_popcount(i);
                    //}
                }
            }
            sum += min;
        }
        std::cout << "Minimum number of buttons: " << sum << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
