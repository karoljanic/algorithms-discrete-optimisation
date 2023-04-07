#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <ostream>
#include <istream>
#include <algorithm>


class Graph {
private:
    unsigned int n;
    unsigned int m;
    bool undirected;

    std::vector<std::vector<unsigned int>> adjacencyList;

public:
    Graph();
    Graph(unsigned int verticesNumber);
    Graph(unsigned int verticesNumber, bool isUndirected);

    void addVertices(unsigned int newVerticesNumber);
    void addEdge(unsigned int v1, unsigned int v2);
    void deleteEgde(unsigned int v1, unsigned int v2);

    void changeToUndirected();
    void changeToDirected();

    void sortVerticesInAdjacencyLists();

    unsigned int getVerticesNumber() const;
    unsigned int getEdgesNumber() const;
    bool isUndirected() const;

    Graph DFS(unsigned int startVertex, std::ostream& out) const;
    Graph BFS(unsigned int startVertex, std::ostream& out) const;
    bool topologicalSort(std::vector<unsigned int>& topologicalOrder) const;
    std::vector<std::vector<unsigned int>> getStronglyConnectedComponents() const;
    bool isBipartite(std::vector<int>& verticesDivision) const;

    void saveWithNetworkXFormat(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const Graph& graph);
    friend std::istream& operator>>(std::istream& in, Graph& graph);

private:
    void getStronglyConnectedComponentsUtil(unsigned int v, unsigned int& dist, 
        std::stack<unsigned int>& verticesStack, std::vector<bool>& vertexOnStack, 
        std::vector<unsigned int>& vertexMinDist, std::vector<unsigned int>& vertexDist, 
        std::vector<std::vector<unsigned int>>& components) const;
};

#endif // GRAPH_H