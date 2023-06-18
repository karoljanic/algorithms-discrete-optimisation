#include "HipercubeGraph.hpp"

#include <random>
#include <algorithm>
#include <iostream>
#include <vector>

HipercubeGraph::HipercubeGraph(uint8_t dimension) : Graph{}, cubeDimension{dimension} {
    verticesNumber = 1 << dimension;    // vertices in [0, ..., 2^dimension - 1]
    adjacencyList.resize(verticesNumber);

    std::mt19937 gen(std::random_device{}());

    for(ssize_t v = 0; v < static_cast<ssize_t>(verticesNumber); v++) {
        for(uint8_t k = 0; k < dimension; k++) {
            ssize_t u = v ^ (1 << k);    // reverse k-th bit of v; x XOR 1 = ~x
            if(u <= v)
                continue;

            uint8_t v_hamming = hammingWeight(v);
            uint8_t u_hamming = hammingWeight(u);

            uint8_t l = std::max(
                std::max(static_cast<int>(v_hamming), dimension - v_hamming), 
                std::max(static_cast<int>(u_hamming), dimension - u_hamming));

            std::uniform_int_distribution<int64_t> dis(1, 1 << l);

            int64_t weight = dis(gen);
            Edge e1{v, u, weight, 0, static_cast<ssize_t>(adjacencyList[u].size())};
            Edge e2{u, v, 0, 0, static_cast<ssize_t>(adjacencyList[v].size())};

            adjacencyList[v].emplace_back(e1);
            adjacencyList[u].emplace_back(e2);
            edgesNumber += 2;
        }
    }
}

uint8_t HipercubeGraph::hammingWeight(size_t value) {
    uint8_t weight = 0;
    while(value != 0) {
        weight += value & 1;
        value >>= 1;
    }

    return weight;
}