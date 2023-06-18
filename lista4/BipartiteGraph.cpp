#include "BipartiteGraph.hpp"

#include <random>
#include <set>

BipartiteGraph::BipartiteGraph(uint8_t size, uint8_t degree) : Graph{}, subsetSize{size}, vertexDegree{degree} {
    // vertices in [0, ..., 2^(size + 1) + 1]; 
    // each subset consist of 2^size elements: [1, 2^size] and [2^size + 1, 2^(size + 1)]
    // source is vertex 0, target is vertex 2^(size + 1) + 1
    verticesNumber = (1 << (size + 1)) + 2;       
    adjacencyList.resize(verticesNumber); 

    ssize_t subsetVerticesCount = 1 << size;

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<ssize_t> dis(subsetVerticesCount + 1, 2 * subsetVerticesCount);

    for(ssize_t v = 1; v <= subsetVerticesCount; v++) {
        ssize_t u = 0;
        Edge e1{v, u, 0, 0, static_cast<ssize_t>(adjacencyList[u].size())};
        Edge e2{u, v, 1, 0, static_cast<ssize_t>(adjacencyList[v].size())};

        adjacencyList[v].emplace_back(e1);
        adjacencyList[u].emplace_back(e2);
        edgesNumber += 2;
    }

    for(ssize_t v = 1; v <= subsetVerticesCount; v++) {
        std::set<ssize_t> s;
        while(s.size() < degree) {
            ssize_t u = dis(gen);
            //if(adjacencyList[u].size() < degree)
            s.insert(u);
        }

        for(auto& u: s) {
            Edge e1{v, u, 1, 0, static_cast<ssize_t>(adjacencyList[u].size())};
            Edge e2{u, v, 0, 0, static_cast<ssize_t>(adjacencyList[v].size())};

            adjacencyList[v].emplace_back(e1);
            adjacencyList[u].emplace_back(e2);
            edgesNumber += 2;
        }
    } 

    for(ssize_t v = subsetVerticesCount + 1; v <= 2 * subsetVerticesCount; v++) {
        ssize_t u = 2 * subsetVerticesCount + 1;
        Edge e1{v, u, 1, 0, static_cast<ssize_t>(adjacencyList[u].size())};
        Edge e2{u, v, 0, 0, static_cast<ssize_t>(adjacencyList[v].size())};

        adjacencyList[v].emplace_back(e1);
        adjacencyList[u].emplace_back(e2);
        edgesNumber += 2;
    }
}
