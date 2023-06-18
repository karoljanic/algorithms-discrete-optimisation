#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "Graph.hpp"

class BipartiteGraph : public Graph {
private:
    uint8_t subsetSize;
    uint8_t vertexDegree;
    
public:
    BipartiteGraph(uint8_t size, uint8_t degree);
};

#endif // BIPARTITE_GRAPH_HPP