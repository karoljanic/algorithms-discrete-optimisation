#include "Graph.hpp"

#include <queue>
#include <algorithm>
#include <limits>
#include <iostream>

Graph::Graph(): adjacencyList{}, verticesNumber{0}, edgesNumber{0} { }

size_t Graph::getVerticesNumber() const { return verticesNumber; }

size_t Graph::getEdgesNumber() const { return edgesNumber; }

std::ostream& operator<<(std::ostream& out, const Graph& graph) {
    for(size_t v = 0; v < graph.adjacencyList.size(); v++) {
        out << v << ":\n";
        for(const auto& u: graph.adjacencyList[v]) {
            out << "  -> " << u.dst << ": c = " << u.capacity << "  f = " << u.flow << std::endl;
        }
    }

    return out;
}

void Graph::printPositiveFlows(std::ostream& out) const {
    for(size_t v = 0; v < adjacencyList.size(); v++) {
        out << v << ":\n";
        for(const auto& u: adjacencyList[v]) {
            if(u.flow > 0)
            out << "  -> " << u.dst << ": f = " << u.flow << std::endl;
        }
    }
}


void Graph::generateGLPKModel(std::ofstream& file) const {
    if(!file.is_open()) {
        return;
    }

    file << "using JuMP\n";
    file << "using GLPK\n\n";
    file << "maxFlow = Model(GLPK.Optimizer)\n\n";
    file << "# f[i, j] - flow from node i to node j - nodes are from 1 to 2^k\n";
    file << "@variable(maxFlow, f[1:" <<  verticesNumber << ", 1:" << verticesNumber <<"] >= 0)\n\n";
    file << "# Capacities of given arc\n";

    std::vector<std::vector<bool>> nonZero = std::vector<std::vector<bool>>(verticesNumber, std::vector<bool>(verticesNumber, false));

    for(auto& edges: adjacencyList) {
        for(auto& edge: edges) {
            if(edge.capacity > 0) {
                nonZero[edge.src][edge.dst] = true;
                file << "@constraint(maxFlow, f[" << edge.src + 1 << ", " << edge.dst + 1 << "] <= " << edge.capacity << ")\n";
            }
        }
    }

    for(size_t i = 0; i < verticesNumber; i++) {
        for(size_t j = 0; j < verticesNumber; j++) {
            if(!nonZero[i][j]) {
                file << "@constraint(maxFlow, f[" << i + 1 << ", " << j + 1 << "] <= 0 )\n";
            }
        }
    }
     
    file << "\n# Flow must be balanced\n";
    file << "@constraint(maxFlow, [i = 2:" << verticesNumber - 1 <<"], sum(f[i, :]) == sum(f[:, i]))\n";
    file << "@constraint(maxFlow, sum(f[1, :]) == sum(f[:, " << verticesNumber <<"]))\n\n";
    file <<  "# Maximize the flow\n";
    file <<  "@objective(maxFlow, Max, sum(f[1, :]))\n";
    file <<  "optimize!(maxFlow)\n\n";
    file << "# Print results\n";
    file <<  "println(objective_value(maxFlow))\n";

    if(verticesNumber > 4)
        return;
        
    file << "for i in 1:" << verticesNumber << "\n";
    file << "    for j in 1:" << verticesNumber << "\n";
    file << "        index =(i - 1) * " << verticesNumber << " + j\n";
    file << "        print(value.(f)[index])\n";
    file << "        print(\"  \")\n";
    file << "    end\n";
    file << "    println(\"\\n\")\n";
    file << "end\n";
}

int64_t Graph::edmondsKarpCalculatePath(ssize_t source, ssize_t target, std::vector<Edge*>& preds) {
    preds = std::vector<Edge*>(verticesNumber, nullptr);
    std::queue<ssize_t> q;

    q.push(source);
    while(!q.empty()) {
        ssize_t tmp = q.front();
        q.pop();

        for(Edge& edge : adjacencyList[tmp]) {
            if(preds[edge.dst] == nullptr && edge.dst != source && edge.capacity > edge.flow) {
                preds[edge.dst] = &edge;
                q.push(edge.dst);
            }
        }
    }

    if(preds[target] == nullptr) {
        return 0;
    }

    int64_t flow = std::numeric_limits<int64_t>::max();
    for(Edge* edge = preds[target]; edge != nullptr; edge = preds[edge->src]) {
        flow = std::min(flow, edge->capacity - edge->flow);
    }

    return flow;
}

std::pair<int64_t, size_t> Graph::edmondsKarpMaxFlow(ssize_t source, ssize_t target) {
    int64_t maxFlow = 0;
    size_t augmentingPathsCounter = 0;

    std::vector<Edge*> preds(verticesNumber, nullptr);

    while(int64_t flow = edmondsKarpCalculatePath(source, target, preds)) {
        for(auto& edge = preds[target]; edge != nullptr; edge = preds[edge->src]) {
            edge->flow += flow;

            if(edge->reversedEdge != -1) {
                adjacencyList[edge->dst][edge->reversedEdge].flow = -flow;
            }
        }

        augmentingPathsCounter++;
        maxFlow += flow;
    }
    
    return std::make_pair(maxFlow, augmentingPathsCounter);
}

bool Graph::dinicCalculateLevels(ssize_t source, ssize_t target, std::vector<ssize_t>& levels) {
    for(size_t i = 0; i < verticesNumber; i++) {
        levels[i] = -1;
    }
 
    levels[source] = 0;
 
    std::queue<ssize_t> q;
    q.push(source);
 
    std::vector<Edge>::iterator i;
    while(!q.empty()) {
        ssize_t u = q.front();
        q.pop();
        for(Edge& e : adjacencyList[u]) {
            if(levels[e.dst] < 0 && e.flow < e.capacity) {
                levels[e.dst] = levels[u] + 1;
                q.push(e.dst);
            }
        }
    }
 
    return levels[target] >= 0;
}

int64_t Graph::dinicSendFlow(ssize_t source, int64_t flow, ssize_t target, std::vector<ssize_t>& start, std::vector<ssize_t>& levels) {
    if(source == target) {
        return flow;
    }
 
    for(; start[source] < static_cast<ssize_t>(adjacencyList[source].size()); start[source]++) {
        Edge& e = adjacencyList[source][start[source]];
 
        if(levels[e.dst] == levels[source] + 1 && e.flow < e.capacity) {
            int64_t currFlow = std::min(flow, e.capacity - e.flow);
            int64_t tempFlow = dinicSendFlow(e.dst, currFlow, target, start, levels);
 
            if(tempFlow > 0) {
                e.flow += tempFlow;
                adjacencyList[e.dst][e.reversedEdge].flow -= tempFlow;

                return tempFlow;
            }
        }
    }
 
    return 0;
}

std::pair<int64_t, size_t> Graph::dinicMaxFlow(ssize_t source, ssize_t target) {
    int64_t maxFlow = 0;
    size_t augmentingPathsCounter = 0;

    if(source == target) {
        return std::make_pair(maxFlow, augmentingPathsCounter);
    }

    std::vector<ssize_t> levels(verticesNumber);
  
    while(dinicCalculateLevels(source, target, levels)) {
        std::vector<ssize_t> start(verticesNumber + 1, 0);
 
        while(int64_t flow = dinicSendFlow(source, std::numeric_limits<int64_t>::max(), target, start, levels)) {
            maxFlow += flow;

            augmentingPathsCounter++;
        }
    }

    return std::make_pair(maxFlow, augmentingPathsCounter);
}
