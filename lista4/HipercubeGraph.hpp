#ifndef HIPERCUBE_GRAPH_HPP
#define HIPERCUBE_GRAPH_HPP

#include "Graph.hpp"

class HipercubeGraph : public Graph {
private:
    uint8_t cubeDimension;

public:
    HipercubeGraph(uint8_t dimension);

private:
    static uint8_t hammingWeight(size_t value);
};

#endif // HIPERCUBE_GRAPH_HPP
