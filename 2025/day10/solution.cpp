#include <algorithm>
#include <bitset>
#include <cstdint>
#include <stdexcept>

#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <string>

#include <tuple>
#include <unordered_set>
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
using data_type = std::tuple<std::bitset<16>, buttons_type, std::vector<u_int16_t>>;

using bitset_vector_type = std::vector<std::bitset<16>>;
//masine launch bitset, vector of buttons bitsets
using diagram_type = std::tuple<std::bitset<16>, bitset_vector_type>;

constexpr int LAUNCH_BITSET = 0;
constexpr int BUTTONS = 1;
constexpr int COUNTERS = 2;

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

        // Parse values from {...}
        std::vector<u_int16_t> values;
        pos = closeBracket;
        size_t openBrace = line.find('{', pos);
        if (openBrace != std::string::npos) {
            size_t closeBrace = line.find('}', openBrace);
            std::string content = line.substr(openBrace + 1, closeBrace - openBrace - 1);
            
            // Parse comma-separated values
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
                    values.push_back(static_cast<u_int16_t>(std::stoul(numStr)));
                }
                numPos = commaPos + 1;
            }
        }

        data.push_back(std::make_tuple(bits, buttons, values));
    }

    inFile.close();
    return data;
}

std::string make_key(const std::vector<u_int16_t>& counters) {
    std::string key;
    key.resize(counters.size() * sizeof(u_int16_t));
    std::memcpy(key.data(), counters.data(), key.size());
    return key;
}

//queue item: target, step, counters
using state_type = std::tuple<u_int16_t, std::vector<u_int16_t>>;

size_t calc_min_steps(const bitset_vector_type &buttons_bits, const std::vector<u_int16_t> &target) {

    size_t num_buttons = buttons_bits.size();
    size_t num_counters = target.size();

    std::vector<u_int16_t> to_process(num_counters, 0);
    std::unordered_set<std::string> visited;
    visited.reserve(1000000);

    //BFS queue
    std::queue<state_type> state_queue;

    //push initial state to the queue
    state_queue.push(std::make_tuple(0, to_process));
    //check if this state was visited
    std::string key = make_key(to_process);
    visited.insert(key);

    size_t max_steps = 1000; // Add a reasonable limit to prevent infinite loops

    bool bOut = true;
    while(!state_queue.empty()) {
        auto [current_step, current_counters] = state_queue.front();
        state_queue.pop();

        if ((current_step % 10 == 0) && bOut) {
            if(bOut) {
                std::cout << "At step " << current_step << ", queue size: " << state_queue.size() << std::endl;
        
                std::cout << "Current step: " << current_step << ", Queue size: " << state_queue.size() << 
                    " Counters: ";
                for(size_t i = 0; i < num_counters; i++) {
                    std::cout << current_counters[i] << " ";
                }
                std::cout << std::endl;
                bOut = false;
            }
            
        } else {
            if (!bOut) bOut = true;
        }

        //try all buttons
        for(size_t i = 0; i < num_buttons; i++) {
            std::vector<u_int16_t> new_counters = current_counters;
            const auto &buttonBitSet = buttons_bits[i];

            //check if we exceeded any target counter
            bool exceeded = false;
            for(size_t i = 0; i < num_counters; i++) {
                if(buttonBitSet[i]) {
                    new_counters[i]++;
                }
                if(new_counters[i] > target[i]) {
                    exceeded = true;
                    break;
                }
            }

            if(exceeded) {
                continue; //skip this state
            }   
        
            //check if current counters match the target
            if(new_counters == target) {
                return current_step + 1; //found solution
            }

            //check if this state was visited
            std::string new_counters_key = make_key(new_counters);
            if(visited.find(new_counters_key) != visited.end()) {
                continue; //skip already visited state
            }

            visited.insert(new_counters_key);

            //push new state to the queue
            state_queue.push(std::make_tuple(current_step + 1, new_counters));

        }
    }   

    return 0;
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
        
        // find minimal number of items to get required combination;
        std::cout << "Calculating minimum number of buttons..." << std::endl;
        int sum = 0;
        for (const auto &mashine : mashines) {

            const auto &buttons = std::get<BUTTONS>(mashine);
            size_t num_buttons = buttons.size();
            int min = INT16_MAX;
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

        //part2
        // find minimal number of items;
        std::cout << "Calculating minimum number of buttons part 2..." << std::endl;
        int sum_target = 0;
        for (const auto &item : data) {
            const auto &buttons = std::get<BUTTONS>(item);
            const auto &target = std::get<COUNTERS>(item);
            
            //output current item
            auto mashine = mashines[&item - &data[0]];  
            std::cout << "Evaluating mashine with launch bitset " << std::get<LAUNCH_BITSET>(mashine) << " and target counters { ";
            for (const auto &t : target) {
                std::cout << t << " ";
            }
            std::cout << "}." << std::endl; 

            bitset_vector_type bits_vector;
            std::cout << " Buttons: { ";
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

            sum_target += calc_min_steps(bits_vector, target);
        }
        //part 1 solution
        std::cout << "Minimum number of buttons: " << sum << std::endl;
        //part 2 solution
        std::cout << "Minimum number of buttons: " << sum_target << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
