#include <iostream>
#include <queue>

#include "Runner.hpp"
#include "Graph.hpp"

class BucketQueue {
    size_t nodesInBucketsNumber;
    size_t bucketsNumber;
    size_t minKeyBucket;
    std::vector<std::queue<size_t>> buckets;

public:
    BucketQueue(int64_t maxWeight) : 
        nodesInBucketsNumber{0}, 
        bucketsNumber{static_cast<size_t>(maxWeight + 1)},
        minKeyBucket{0},
        buckets{} 
    {
        buckets.resize(bucketsNumber);
    }

    bool isEmpty() const { return nodesInBucketsNumber == 0; } 

    void push(size_t v, int64_t d) {
        buckets[d % bucketsNumber].push(v);
        nodesInBucketsNumber++;
    }

    size_t pop() {
        while(buckets[minKeyBucket].empty()) {
            minKeyBucket++;
            if(minKeyBucket == bucketsNumber) {
                minKeyBucket = 0;
            }
        }

        size_t v = buckets[minKeyBucket].front();
        buckets[minKeyBucket].pop();
        nodesInBucketsNumber--;

        return v;
    }
};


std::vector<int64_t> shortestPathsDial(const Graph& graph, const size_t source) {
    std::vector<int64_t> shortesPaths(graph.getVerticesNumber(), std::numeric_limits<int64_t>::max());
    std::vector<bool> visited(graph.getVerticesNumber(), false);

    BucketQueue bucketQueue(graph.getMaxWeight());

    shortesPaths[source] = 0;
    bucketQueue.push(source, 0);

    while(!bucketQueue.isEmpty()) {
        size_t v = bucketQueue.pop();
        
        if(visited[v]) {
            continue;
        }
        visited[v] = true;

        for(const auto& neighbor : graph.getNeighbours(v)) {
            int64_t newDist = shortesPaths[v] + neighbor.weight;

            if(shortesPaths[neighbor.vertex] > newDist) {
                shortesPaths[neighbor.vertex] = newDist;
                bucketQueue.push(neighbor.vertex, newDist);
            }
        }
    }

    return shortesPaths;
}

int main(int argc, char* argv[]) {
    int status = run(argc, argv, "dial", shortestPathsDial);

    return status;
}