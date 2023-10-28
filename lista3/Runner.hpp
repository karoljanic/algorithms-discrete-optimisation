#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <iostream>
#include <fstream>
#include <string.h>
#include <functional>
#include <chrono>

#include "Graph.hpp"

constexpr int ExpectedArgNumber = 7;


// return 0 - OK
// return 1 - invalid arguments number
// return 2 - invalid arguments
// return 3 - cannot access files
int run(int argc, char* argv[], const char* algorithmName, std::function<std::vector<int64_t>(const Graph&, const size_t)> shortestPathsAlgorithm) {
    if(argc != ExpectedArgNumber)
        return 1;
    
    std::ifstream dataFile{argv[2]};
    std::ifstream inputFile{argv[4]};
    std::ofstream outputFile{argv[6]};

    if(!dataFile.is_open() || !inputFile.is_open() || !outputFile.is_open())
        return 3;

    if(strcmp(argv[1], "-d") != 0) {
        return 2;
    }

    Graph graph;
    dataFile >> graph;

    if(strcmp(argv[3], "-ss") == 0 && strcmp(argv[5], "-oss") == 0) {
        std::string line;
        std::string tmp;
        size_t source;
        std::vector<size_t> sources;

        while(std::getline(inputFile, line)) {
            if (line[0] == 's') {  
                std::stringstream ss(line);
                ss >> tmp >> source;
                sources.emplace_back(source);
            }
        }

        auto start = std::chrono::high_resolution_clock::now();
        for(const auto& s: sources) {
            shortestPathsAlgorithm(graph, s - 1);
        }
        auto stop = std::chrono::high_resolution_clock::now();

        float duration = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count());
        float meanDuration = duration / sources.size();

        outputFile << "p res sp ss " << algorithmName << std::endl;
        outputFile << "f " << argv[2] << " " << argv[4] << std::endl;
        outputFile << "g " << graph.getVerticesNumber() << " " << graph.getEdgesNumber() << " " << graph.getMinWeight() << " " << graph.getMaxWeight() << std::endl;
        outputFile << "t " << meanDuration / 1000 << std::endl;
    }
    else if(strcmp(argv[3], "-p2p") == 0 && strcmp(argv[5], "-op2p") == 0) {
        std::string line;
        std::string tmp;
        size_t v1;
        size_t v2;
        std::vector<std::pair<size_t, size_t>> pointPairs;

        while(std::getline(inputFile, line)) {
            if (line[0] == 'q') {  
                std::stringstream ss(line);
                ss >> tmp >> v1 >> v2;
                pointPairs.emplace_back(v1, v2);
            }
        }

        outputFile << "f " << argv[2] << " " << argv[4] << std::endl;
        outputFile << "g " << graph.getVerticesNumber() << " " << graph.getEdgesNumber() << " " << graph.getMinWeight() << " " << graph.getMaxWeight() << std::endl;
    
        for(const auto& pair: pointPairs) {
            std::vector<int64_t> shortestPaths = shortestPathsAlgorithm(graph, pair.first - 1);
            outputFile << "d " << pair.first << " " << pair.second << " " << shortestPaths[pair.second - 1] << std::endl;
        }
    }
    else
        return 2;


    return 0;
}

#endif // RUNNER_HPP