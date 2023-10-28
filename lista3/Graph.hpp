#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <cstdint>
#include <vector>
#include <istream>
#include <ostream>
#include <sstream>
#include <limits>
#include <iostream>

#include "Graph.hpp"

class Graph {
private:
    struct Edge {
        size_t vertex;
        int64_t weight;
    };

    size_t verticesNumber;
    size_t edgesNumber;
    int64_t minWeight;
    int64_t maxWeight;
    std::vector<std::vector<Edge>> adjacencyList;

public:
    Graph();
    Graph(size_t n);

    size_t getVerticesNumber() const;
    size_t getEdgesNumber() const;
    int64_t getMinWeight() const;
    int64_t getMaxWeight() const;
    const std::vector<Edge>& getNeighbours(size_t v) const;
    void resize(size_t n);
    void addEdge(size_t v1, size_t v2, int64_t w);

    friend std::istream& operator>>(std::istream& in, Graph& graph);
    friend std::ostream& operator<<(std::ostream& out, const Graph& graph);
};

#endif // GRAPH_HPP
