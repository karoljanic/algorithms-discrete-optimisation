#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include "Graph.hpp"

class BipartiteGraph : public Graph {
private:
    size_t subsetSize;
    size_t vertexDegree;
    
public:
    BipartiteGraph(size_t size, size_t degree);
};

#endif // BIPARTITE_GRAPH_HPP