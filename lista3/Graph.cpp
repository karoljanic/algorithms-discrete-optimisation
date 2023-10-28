#include "Graph.hpp"

Graph::Graph(): 
    verticesNumber{0}, 
    edgesNumber{0},
    minWeight{std::numeric_limits<int64_t>::max()},
    maxWeight{std::numeric_limits<int64_t>::min()},
    adjacencyList{} { }

Graph::Graph(size_t n): 
    verticesNumber{n},
    edgesNumber{0},
    minWeight{std::numeric_limits<int64_t>::max()},
    maxWeight{std::numeric_limits<int64_t>::min()},
    adjacencyList{} { }

size_t Graph::getVerticesNumber() const {
    return verticesNumber;
}

size_t Graph::getEdgesNumber() const {
    return edgesNumber;
}

int64_t Graph::getMinWeight() const {
    return minWeight;
}

int64_t Graph::getMaxWeight() const {
    return maxWeight;
}

const std::vector<Graph::Edge>& Graph::getNeighbours(size_t v) const {
    return adjacencyList[v];
}

void Graph::resize(size_t n) {
    verticesNumber = n;
    adjacencyList.resize(n, { });
}

void Graph::addEdge(size_t v1, size_t v2, int64_t w) {
    if(w < minWeight) {
        minWeight  = w;
    }
    else if(w > maxWeight) {
        maxWeight = w;
    }

    adjacencyList[v1 - 1].emplace_back(v2 - 1, w);
}

std::istream& operator>>(std::istream& in, Graph& graph) {
    graph.minWeight = std::numeric_limits<int64_t>::max();
    graph.maxWeight = std::numeric_limits<int64_t>::min();

    std::string line;
    std::string tmp;
    size_t totalVertices;
    size_t totalEdges = 1;
    size_t edgeCounter = 0;

    while(std::getline(in, line) && edgeCounter < totalEdges) {
        switch (line[0]) {
            case 'p': {
                std::stringstream ss(line);
                ss >> tmp >> tmp >> totalVertices >> totalEdges;
                graph.resize(totalVertices);

                break;
            }
            case 'a': {
                size_t v1;
                size_t v2;
                int64_t w;
                std::stringstream ss(line);
                ss >> tmp >> v1 >> v2 >> w;
                graph.addEdge(v1, v2, w);

                edgeCounter++;
                if (w > graph.maxWeight) {
                    graph.maxWeight = w;
                }
                else if (w < graph.minWeight) {
                    graph.minWeight = w;
                }
                
                ss.str("");
                break;
            }
        }
    }

    graph.verticesNumber = totalVertices;
    graph.edgesNumber = totalEdges;

    return in;
}

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
    for(size_t v1 = 0; v1 < graph.verticesNumber; v1++) {
        out << (v1 + 1) << ": ";
        for(const auto& e: graph.adjacencyList[v1]) {
            out << "(" << (e.vertex + 1) << ", " << e.weight << "), ";
        }
        out << '\n';
    }

    return out;
}