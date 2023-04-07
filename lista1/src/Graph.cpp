#include "Graph.h"

Graph::Graph() 
    : n{0}, m{0}, undirected{true}, adjacencyList{0} { }

Graph::Graph(unsigned int verticesNumber) 
    : n{verticesNumber}, m{0}, undirected{true}, adjacencyList{verticesNumber + 1} { }

Graph::Graph(unsigned int verticesNumber, bool isUndirected) 
    : n{verticesNumber}, m{0}, undirected{isUndirected}, adjacencyList{verticesNumber + 1} { }

void Graph::addVertices(unsigned int newVerticesNumber) {
    n += newVerticesNumber;
    adjacencyList.resize(n);
}

void Graph::addEdge(unsigned int v1, unsigned int v2) { 
    if(undirected) {
        m += 2;
        adjacencyList[v1].push_back(v2);
        adjacencyList[v2].push_back(v1);
    }
    else {
        m++;
        adjacencyList[v1].push_back(v2);
    }
}

void Graph::deleteEgde(unsigned int v1, unsigned int v2) {
    if(undirected) {
        m -= 2;
        adjacencyList[v1].erase(std::find(adjacencyList[v1].begin(), adjacencyList[v1].end(), v2));
        adjacencyList[v2].erase(std::find(adjacencyList[v2].begin(), adjacencyList[v2].end(), v1));
    }
    else {
        m--;
        adjacencyList[v1].erase(std::find(adjacencyList[v1].begin(), adjacencyList[v1].end(), v2));
    }
}

void Graph::changeToUndirected() {
    undirected = true;
}

void Graph::changeToDirected() {
    undirected = false;
}
 
void Graph::sortVerticesInAdjacencyLists() {
    for(unsigned int v = 1; v <= n; v++) {
        std::sort(adjacencyList[v].rbegin(), adjacencyList[v].rend());
    }
}

unsigned int Graph::getVerticesNumber() const {
    return n;
}

unsigned int Graph::getEdgesNumber() const { 
    return m;
}

bool Graph::isUndirected() const {
    return undirected;
}

Graph Graph::DFS(unsigned int startVertex, std::ostream& out) const {
    Graph searchTree(n, false);
    
    std::stack<unsigned int> verticesToProcess;
    std::vector<bool> visited(n + 1, false);

    visited[startVertex] = true;
    verticesToProcess.push(startVertex);

    unsigned int tmp;
    while(!verticesToProcess.empty()) {
        tmp = verticesToProcess.top();
        verticesToProcess.pop();

        out << tmp << " ";

        for(const unsigned int& neighbour : adjacencyList[tmp]) {
            if(!visited[neighbour]) {
                verticesToProcess.push(neighbour);
                visited[neighbour] = true;
                searchTree.addEdge(tmp, neighbour);
            }
        }
    }

    out << std::endl;

    return searchTree;
}

Graph Graph::BFS(unsigned int startVertex, std::ostream& out) const {
    Graph searchTree(n, false);
    
    std::queue<unsigned int> verticesToProcess;
    std::vector<bool> visited(n + 1, false);

    visited[startVertex] = true;
    verticesToProcess.push(startVertex);

    unsigned int tmp = startVertex;
    while(!verticesToProcess.empty()) {
        tmp = verticesToProcess.front();
        verticesToProcess.pop();

        out << tmp << " ";

        for(const unsigned int& neighbour : adjacencyList[tmp]) {
            if(!visited[neighbour]) {
                verticesToProcess.push(neighbour);
                visited[neighbour] = true;
                searchTree.addEdge(tmp, neighbour);
            }
        }
    }

    out << std::endl;

    return searchTree;
}

bool Graph::topologicalSort(std::vector<unsigned int>& topologicalOrder) const {
    std::vector<unsigned int> indegreeCount(n + 1, 0);

    for(unsigned int v1 = 1; v1 <= n; v1++) {
        for(const unsigned int& v2 : adjacencyList[v1]) {
            indegreeCount[v2]++;
        }
    }

    std::queue<unsigned int> isolated;
    for(unsigned int v = 1; v < n; v++) {
        if(indegreeCount[v] == 0) {
            isolated.push(v);
        }
    }
    
    topologicalOrder.reserve(n);

    unsigned int visitedNodes = 0;
    while(!isolated.empty()) {
        unsigned int tmp = isolated.front();
        isolated.pop();
        topologicalOrder.push_back(tmp);

        for(const unsigned int& v : adjacencyList[tmp]) {
            indegreeCount[v]--;
            if(indegreeCount[v] == 0) {
                isolated.push(v);
            }
        }

        visitedNodes++;
    }

    return (visitedNodes == n);
}

std::vector<std::vector<unsigned int>> Graph::getStronglyConnectedComponents() const {
    std::vector<std::vector<unsigned int>> components;

    std::stack<unsigned int> verticesStack;
    std::vector<unsigned int> vertexDist(n + 1, 0);
    std::vector<unsigned int>  vertexMinDist(n + 1, 0);
    std::vector<bool> vertexOnStack(n + 1, false);
    unsigned int dist = 0;

    for(unsigned int v = 1; v <= n; v++) {
        if(vertexDist[v] == 0) {
            getStronglyConnectedComponentsUtil(v, dist, verticesStack, vertexOnStack, vertexMinDist, vertexDist, components);
        }
    }
    
    return components;
}

void Graph::getStronglyConnectedComponentsUtil(unsigned int v, unsigned int& dist, std::stack<unsigned int>& verticesStack, std::vector<bool>& vertexOnStack, std::vector<unsigned int>& vertexMinDist, std::vector<unsigned int>& vertexDist, std::vector<std::vector<unsigned int>>& components) const {
    dist++;
    vertexDist[v] = dist;
    vertexMinDist[v] = dist;

    verticesStack.push(v);
    vertexOnStack[v] = true;

    for(const unsigned int& u : adjacencyList[v]) {
        if(vertexDist[u] == 0) {
            getStronglyConnectedComponentsUtil(u, dist, verticesStack, vertexOnStack, vertexMinDist, vertexDist, components);
            vertexMinDist[v] = std::min(vertexMinDist[v], vertexMinDist[u]);
        }
        else if(vertexOnStack[u]) {
            vertexMinDist[v] = std::min(vertexMinDist[v], vertexDist[u]);
        }
    }

    if(vertexMinDist[v] == vertexDist[v]) {
        std::vector<unsigned int> component;

        unsigned int w;
        do {
            w = verticesStack.top();
            verticesStack.pop();
            vertexOnStack[w] = false;
            component.push_back(w);
        }
        while(w != v);

        components.emplace_back(std::move(component));
    }
}

bool Graph::isBipartite(std::vector<int>& verticesDivision) const {
    verticesDivision.resize(n + 1, 0);

    std::queue<unsigned int> verticesToProcess;

    for(unsigned int v1 = 1; v1 <= n; v1++) {
        if(verticesDivision[v1] == 0) {
            verticesDivision[v1] = 1;
            verticesToProcess.push(v1);

            while(!verticesToProcess.empty()) {
                unsigned int v2 = verticesToProcess.front();
                verticesToProcess.pop();

                for(const unsigned int& v3 : adjacencyList[v2]) {
                    if(verticesDivision[v2] == verticesDivision[v3]) {
                        return false;
                    }

                    if(verticesDivision[v3] == 0) {
                        verticesDivision[v3] = -verticesDivision[v2];
                        verticesToProcess.push(v3);
                    }
                }
            }
        }
    }  

    return true;  
}

void Graph::saveWithNetworkXFormat(std::ostream& out) const {
    out << "source,target\n";
    for(unsigned int v1 = 1; v1 <= n; v1++) {
        for(const unsigned int& neighbour : adjacencyList[v1]) {
            out << v1 << "," << neighbour << "\n";
        }
    }
}

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
    for(unsigned int v1 = 1; v1 <= graph.n; v1++) {
        out << v1 << ": ";
        for(const unsigned int& neighbour : graph.adjacencyList[v1]) {
            out << neighbour << " ";
        }
        out << "\n";
    }

    return out;
}

std::istream& operator>>(std::istream& in, Graph& graph) {
    char graphType;
    in >> graphType;

    if(graphType == 'U') {
        graph.undirected = true;
    }
    else if(graphType == 'D') {
        graph.undirected = false;
    }
    else {
        throw std::invalid_argument("Invalid graph type!");
    }

    try {
        in >> graph.n;
        graph.adjacencyList = std::vector<std::vector<unsigned int>>(graph.n + 1);
    }
    catch(const std::exception& e) {
        throw std::invalid_argument("Invalid graph vertices number!");
    }

    try {
        in >> graph.m;
    }
    catch(const std::exception& e) {
        throw std::invalid_argument("Invalid graph edges number!");
    }

    try {
        if(graph.undirected) {
            unsigned int v1, v2;
            for(unsigned int i = 0; i < graph.m; i++) {
                in >> v1;
                in >> v2;

                graph.adjacencyList[v1].push_back(v2);
                graph.adjacencyList[v2].push_back(v1);
            }
        }
        else {
            unsigned int v1, v2;
            for(unsigned int i = 0; i < graph.m; i++) {
                in >> v1;
                in >> v2;

                graph.adjacencyList[v1].push_back(v2);
            }
        }
    }
    catch(const std::exception& e) {
        throw std::invalid_argument("Invalid grap edges description!");
    }

    return in;
}