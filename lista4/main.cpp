#include <iostream>
#include <cstring>
#include <fstream>

#include "HipercubeGraph.hpp"
#include "BipartiteGraph.hpp"

int main(int argc, char* argv[]) {
    // ./main --cube --size k --printFlow
    // ./main --bi --size k --degree i --printMatching 
    // ./main --dinic --size k --printFlow
    
    //auto start = std::chrono::system_clock::now();
    //int result = 0;
    //int sciezki_powiekszajace_count = 0;

    std::ofstream file{"out.jl"};

    if(argc >= 3 && strcmp(argv[1], "--cube") == 0) {
        uint8_t k = std::stoi(argv[3]);
        bool print = (argc == 4);

        auto cube = HipercubeGraph(k);
        cube.generateGLPKModel(file);
        std::cout << cube.getVerticesNumber() << " " << cube.getEdgesNumber() << std::endl;
        std::cout << cube << std::endl;

        auto result = cube.edmondsKarpMaxFlow(0, (1 << k) - 1);
        std::cout << result.first << " " << result.second << std::endl;
    }
    else if(argc >= 5 && strcmp(argv[1], "--bi") == 0) {
        uint8_t k = std::stoi(argv[3]);
        uint8_t i = std::stoi(argv[5]);
        bool print = (argc == 6);

        auto bi = BipartiteGraph(k , i);
        bi.generateGLPKModel(file);
        std::cout << bi.getVerticesNumber() << " " << bi.getEdgesNumber() << std::endl;
        std::cout << bi << std::endl;

        auto result = bi.edmondsKarpMaxFlow(0, (1 << (k + 1)) + 1);
        std::cout << result.first << " " << result.second << std::endl;
    }
    else if(argc >= 3 && strcmp(argv[1], "--dinic") == 0) {
        uint8_t k = std::stoi(argv[3]);
        bool print = (argc == 4);

        auto cube = HipercubeGraph(k);
        cube.generateGLPKModel(file);
        std::cout << cube.getVerticesNumber() << " " << cube.getEdgesNumber() << std::endl;
        std::cout << cube << std::endl;

        auto result = cube.dinicMaxFlow(0, (1 << k) - 1);
        std::cout << result.first << " " << result.second << std::endl;
    }

    //auto end = std::chrono::system_clock::now();

    //std::cout<<"Runtime was: "<<std::chrono::duration_cast<std::chrono::seconds>(end - start).count()<<std::endl;
    //std::cout<<"Number of ścieżki powiększające: "<<sciezki_powiekszajace_count<<std::endl;

    return 0;
}