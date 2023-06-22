#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <chrono>

#include "HipercubeGraph.hpp"
#include "BipartiteGraph.hpp"

    // ./main --cube --size k --printFlow --glpk fileName
    // ./main --bi --size k --degree i --printMatching --glpk fileName
    // ./main --dinic --size k --printFlow --glpk fileName

    // ./main gen-cube-ed
    // ./main gen-bi-ed
    // ./main gen-cube-di

int main(int argc, char* argv[]) {
    if(argc < 2) { return 0; }

    std::vector<std::string> arguments;
    for (int i = 1; i < argc; ++i) {
        arguments.emplace_back(argv[i]);
    }

    if(arguments[0] == "--cube") {
        auto sizeIt = std::find(arguments.begin(), arguments.end(), "--size");
        if(sizeIt == arguments.end()) { return 0; }

        auto sizeValIt = std::next(sizeIt);
        if(sizeValIt == arguments.end()) { return 0; }

        uint8_t k;
        try { k = std::stoi(*sizeValIt); }
        catch(const std::invalid_argument& e) { return 0; }

        auto printFlowIt = std::find(arguments.begin(), arguments.end(), "--printFlow");
        bool printFlow = (printFlowIt != arguments.end());

        auto glpkIt = std::find(arguments.begin(), arguments.end(), "--glpk");
        bool saveToFile = (glpkIt != arguments.end());

        std::ofstream file;
        if(saveToFile) {
            auto glpkFileNameIt = std::next(glpkIt);
            if(glpkFileNameIt == arguments.end()) { return 0; }
            file = std::ofstream{*glpkFileNameIt};
        }

        auto cube = HipercubeGraph(k);
        if(saveToFile) { cube.generateGLPKModel(file); }

        auto start = std::chrono::system_clock::now();
        auto res = cube.edmondsKarpMaxFlow(0, (1 << k) - 1);
        auto end = std::chrono::system_clock::now();

        std::cout << res.first << std::endl;
        if(printFlow) {
            cube.printPositiveFlows(std::cout);
            std::cout << std::endl;
        }

        std::cerr << static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0f << " milis" << std::endl;
        std::cerr << res.second << std::endl;
    }
    else if(arguments[0] == "--bi") {
        auto sizeIt = std::find(arguments.begin(), arguments.end(), "--size");
        if(sizeIt == arguments.end()) { return 0; }

        auto sizeValIt = std::next(sizeIt);
        if(sizeValIt == arguments.end()) { return 0; }

        uint8_t k;
        try { k = std::stoi(*sizeValIt); }
        catch(const std::invalid_argument& e) { return 0; }

        auto degreeIt = std::find(arguments.begin(), arguments.end(), "--size");
        if(degreeIt == arguments.end()) { return 0; }

        auto degreeValIt = std::next(degreeIt);
        if(degreeValIt == arguments.end()) { return 0; }

        uint8_t i;
        try { i = std::stoi(*degreeValIt); }
        catch(const std::invalid_argument& e) { return 0; }

        auto printMatchingIt = std::find(arguments.begin(), arguments.end(), "--printMatching");
        bool printMatching = (printMatchingIt != arguments.end());

        auto glpkIt = std::find(arguments.begin(), arguments.end(), "--glpk");
        bool saveToFile = (glpkIt != arguments.end());

        std::ofstream file;
        if(saveToFile) {
            auto glpkFileNameIt = std::next(glpkIt);
            if(glpkFileNameIt == arguments.end()) { return 0; }
            file = std::ofstream{*glpkFileNameIt};
        }

        auto bi = BipartiteGraph(k, i);
        if(saveToFile) { bi.generateGLPKModel(file); }

        auto start = std::chrono::system_clock::now();
        auto res = bi.edmondsKarpMaxFlow(0, (1 << (k + 1)) + 1);
        auto end = std::chrono::system_clock::now();

        std::cout << res.first << std::endl;
        if(printMatching) {
            bi.printPositiveFlows(std::cout);
            std::cout << std::endl;
        }
        
        std::cerr << static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0f << " milis" << std::endl;
        std::cerr << res.second << std::endl;
    }
    else if(arguments[0] == "--dinic") {
        auto sizeIt = std::find(arguments.begin(), arguments.end(), "--size");
        if(sizeIt == arguments.end()) { return 0; }

        auto sizeValIt = std::next(sizeIt);
        if(sizeValIt == arguments.end()) { return 0; }

        uint8_t k;
        try { k = std::stoi(*sizeValIt); }
        catch(const std::invalid_argument& e) { return 0; }

        auto printFlowIt = std::find(arguments.begin(), arguments.end(), "--printFlow");
        bool printFlow = (printFlowIt != arguments.end());

        auto glpkIt = std::find(arguments.begin(), arguments.end(), "--glpk");
        bool saveToFile = (glpkIt != arguments.end());

        std::ofstream file;
        if(saveToFile) {
            auto glpkFileNameIt = std::next(glpkIt);
            if(glpkFileNameIt == arguments.end()) { return 0; }
            file = std::ofstream{*glpkFileNameIt};
        }

        auto cube = HipercubeGraph(k);
        if(saveToFile) { cube.generateGLPKModel(file); }

        auto start = std::chrono::system_clock::now();
        auto res = cube.dinicMaxFlow(0, (1 << k) - 1);
        auto end = std::chrono::system_clock::now();

        std::cout << res.first << std::endl;
        if(printFlow) {
            cube.printPositiveFlows(std::cout);
            std::cout << std::endl;
        }
        
        std::cerr << static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0f << " milis" << std::endl;
        std::cerr << res.second << std::endl;
    }
    else if(arguments[0] == "gen-cube-ed") {
        std::ofstream file{"gen-cube-ed.txt"};
        constexpr size_t repeats = 10;

        for(size_t k = 16; k <= 16; k++) {
            uint64_t flowSum = 0;
            size_t pathsSum = 0;
            float timesSum = 0;

            std::cout << k << std::endl;

            for(size_t rep = 0; rep < repeats; rep++) {
                std::cout << "rep = " << rep << std::endl;
                auto cube = HipercubeGraph(k);

                auto start = std::chrono::system_clock::now();
                auto res = cube.edmondsKarpMaxFlow(0, (1 << k) - 1);
                auto end = std::chrono::system_clock::now();

                flowSum += res.first;
                pathsSum += res.second;
                timesSum += static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0f;
            }

            file << k << " ";
            file << static_cast<float>(flowSum) / static_cast<float>(repeats) << " ";
            file << static_cast<float>(pathsSum) / static_cast<float>(repeats) << " ";
            file << static_cast<float>(timesSum) / static_cast<float>(repeats) << std::endl;
        }
    }
    else if(arguments[0] == "gen-bi-di") {
        std::ofstream file{"gen-bi-di.txt"};
        constexpr size_t repeats = 10;

        for(size_t k = 3; k <= 10; k++) {
            for(size_t i = 1; i <= k; i++) {
                uint64_t flowSum = 0;
                size_t pathsSum = 0;
                float timesSum = 0;

                std::cout << k << " " << i << std::endl;

                for(size_t rep = 0; rep < repeats; rep++) {
                    auto bipartite = BipartiteGraph(k, i);

                    auto start = std::chrono::system_clock::now();
                    auto res = bipartite.dinicMaxFlow(0, (1 << (k + 1)) + 1);
                    auto end = std::chrono::system_clock::now();

                    flowSum += res.first;
                    pathsSum += res.second;
                    timesSum += static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0f;
                }
                
                file << k << " " << i << " ";
                file << static_cast<float>(flowSum) / static_cast<float>(repeats) << " ";
                file << static_cast<float>(pathsSum) / static_cast<float>(repeats) << " ";
                file << static_cast<float>(timesSum) / static_cast<float>(repeats) << " ";
                file << std::endl;
            }
        }
    }
    else if(arguments[0] == "gen-cube-di") {
        std::ofstream file{"gen-cube-di.txt"};
        constexpr size_t repeats = 10;

        for(size_t k = 1; k <= 16; k++) {
            uint64_t flowSum = 0;
            size_t pathsSum = 0;
            float timesSum = 0;

            std::cout << k << std::endl;

            for(size_t rep = 0; rep < repeats; rep++) {
                auto cube = HipercubeGraph(k);

                auto start = std::chrono::system_clock::now();
                auto res = cube.dinicMaxFlow(0, (1 << k) - 1);
                auto end = std::chrono::system_clock::now();

                flowSum += res.first;
                pathsSum += res.second;
                timesSum += static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0f;
            }

            file << k << " ";
            file << static_cast<float>(flowSum) / static_cast<float>(repeats) << " ";
            file << static_cast<float>(pathsSum) / static_cast<float>(repeats) << " ";
            file << static_cast<float>(timesSum) / static_cast<float>(repeats) << std::endl;
        }
    }

    return 0;
}