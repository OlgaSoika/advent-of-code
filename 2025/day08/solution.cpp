#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <set>
#include <map>

#include <sstream>
#include <utility>
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

std::vector<std::tuple<unsigned long long, unsigned long long, unsigned long long>> readInputFile(const std::string &filename) {
    std::vector<std::tuple<unsigned long long, unsigned long long, unsigned long long>> data;
    std::ifstream inFile(filename);
    
    if (!inFile) {
        throw std::runtime_error("Cannot open the file: " + filename);
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) {
            std::stringstream ss(line);
            unsigned long long x, y, z;
            char comma;
            
            if (ss >> x >> comma >> y >> comma >> z) {
                data.push_back(std::make_tuple(x, y, z));
            }
        }
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
        std::vector<std::tuple<unsigned long long, unsigned long long, unsigned long long>> data = readInputFile(filename);
        
        std::cout << "Read " << data.size() << " lines from file." << std::endl;

              
        // Ask for number of cables
        int num_cables = 10; // default value
        std::cout << "Enter number of cables (default 10, puzzle input 1000): ";
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            try {
                num_cables = std::stoi(input);
            } catch (const std::exception &) {
                std::cout << "Invalid input, using default value: " << num_cables << std::endl;
            }
        }
        std::cout << "Using " << num_cables << " cables" << std::endl;
        
        // solution

        //list of sets of box idxs
        std::list<std::set<size_t>> circuits;

        // Initialize: each junction box starts in its own circuit
        for (size_t i = 0; i < data.size(); i++) {
            std::set<size_t> single_circuit;
            single_circuit.insert(i);
            circuits.push_back(single_circuit);
        }

        //calculate distances without sqrt and save them in a multimap: distance -> (item1, item2)

        std::multimap<unsigned long long, std::pair<size_t, size_t>> distances;

        for (size_t i = 0; i < data.size(); i++) {
            auto [x1, y1, z1] = data[i];

            for (size_t j = i+1; j < data.size(); j++) {
                auto [x2, y2, z2] = data[j];
                
                unsigned long long distance = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2);
                
                distances.insert({distance, {i, j}});
            }
        }

        //now iterate through the distances and group items into circuits
        size_t i = 0;
        while (i < num_cables && i < distances.size()) {
            auto dist_iter = std::next(distances.begin(), i);
            auto [dist, indexes] = *dist_iter;
            auto [item_idx1, item_idx2] = indexes;
            i++;

            auto circuit_it1 = std::find_if( circuits.begin(), circuits.end(),
                                                [&](const std::set<size_t>& c){ return c.count(item_idx1) > 0; }
                                            );
            auto circuit_it2 = std::find_if( circuits.begin(), circuits.end(),
                                                [&](const std::set<size_t>& c){ return c.count(item_idx2) > 0; }
                                            );

            //std::cout << "[DEBUG:" << i << ":1]   Item " << item_idx1 << " found: " << (circuit_it1 != circuits.end() ? "YES" : "NO") << std::endl;
            //std::cout << "[DEBUG:" << i << ":2]   Item " << item_idx2 << " found: " << (circuit_it2 != circuits.end() ? "YES" : "NO") << std::endl;

            if(circuit_it2 == circuit_it1) {
                //both indexes are already in the same circuit, do nothing
                continue;
            }

            //indexes are in different circuits, merge them
            circuit_it1->insert(circuit_it2->begin(), circuit_it2->end());
            circuits.erase(circuit_it2);
        }
        
        std::cout << "Number of circuits: " << circuits.size() << std::endl;
        
        // Collect circuit sizes
        std::vector<size_t> circuit_sizes;
        for (const auto& circuit : circuits) {
            circuit_sizes.push_back(circuit.size());
        }

        unsigned long long circuit_mul = 1;
        std::sort(circuit_sizes.begin(), circuit_sizes.end(), std::greater<size_t>());

        std::cout << "Top 3 circuit sizes: ";
        for (size_t i = 0; i < std::min(size_t(3), circuit_sizes.size()); i++) {
            std::cout << circuit_sizes[i] << " ";
            circuit_mul *= circuit_sizes[i];
        }
        std::cout << std::endl;

        std::cout << "Multiplication of sizes of three the largest circuits: " << circuit_mul << std::endl;
        
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
