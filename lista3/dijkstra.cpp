#include <iostream>
#include <queue>
#include <utility>

#include "Runner.hpp"

typedef std::pair<int64_t, size_t> TmpDist;
typedef std::priority_queue<TmpDist, std::vector<TmpDist>, std::greater<TmpDist>> MinPriorityQueue;

std::vector<int64_t> shortestPathsDijkstra(const Graph& graph, const size_t source) {
    std::vector<int64_t> shortesPaths(graph.getVerticesNumber(), std::numeric_limits<int64_t>::max());
    MinPriorityQueue minPriorityQueue;

    shortesPaths[source] = 0;
    minPriorityQueue.emplace(0, source);

    while(!minPriorityQueue.empty()) {
        int64_t d = minPriorityQueue.top().first;
        size_t v = minPriorityQueue.top().second;
        minPriorityQueue.pop();

        if(d > shortesPaths[v]) {
            continue;
        }

        for(const auto& neighbor : graph.getNeighbours(v)) {
            int64_t newDist = d + neighbor.weight;
 
            if(shortesPaths[neighbor.vertex] > newDist) {
                shortesPaths[neighbor.vertex] = newDist;
                minPriorityQueue.emplace(newDist, neighbor.vertex);
            }
        }
    }

    return shortesPaths;
}

int main(int argc, char* argv[]) {
    int status = run(argc, argv, "dijkstra", shortestPathsDijkstra);

    return status;
}   