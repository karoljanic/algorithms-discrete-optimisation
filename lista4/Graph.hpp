#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <stdint.h>
#include <cstddef>
#include <vector>
#include <ostream>
#include <fstream>

struct Edge {
    ssize_t src = -1;
    ssize_t dst = -1;
    int64_t capacity = 0;
    int64_t flow = 0;
    ssize_t reversedEdge = -1;
};


class Graph {
protected:
    std::vector<std::vector<Edge>> adjacencyList;
    size_t verticesNumber;
    size_t edgesNumber;

public:
    Graph();
    size_t getVerticesNumber() const;
    size_t getEdgesNumber() const;
    void generateGLPKModel(std::ofstream& file) const;

    int64_t edmondsKarpCalculatePath(ssize_t source, ssize_t target, std::vector<Edge*>& preds);
    std::pair<int64_t, size_t> edmondsKarpMaxFlow(ssize_t source, ssize_t target);

    bool dinicCalculateLevels(ssize_t source, ssize_t target, std::vector<ssize_t>& levels);
    int64_t dinicSendFlow(ssize_t source, int64_t flow, ssize_t target, std::vector<ssize_t>& start, std::vector<ssize_t>& levels);
    std::pair<int64_t, size_t> dinicMaxFlow(ssize_t source, ssize_t target);

    friend std::ostream& operator<<(std::ostream& out, const Graph& graph);
};

#endif //GRAPH_HPP