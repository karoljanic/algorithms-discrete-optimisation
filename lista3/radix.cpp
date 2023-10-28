#include <iostream>
#include <array>

#include "Runner.hpp"
#include "Graph.hpp"

typedef std::pair<int64_t, size_t> TmpDist;

constexpr size_t maxWeightNumberBits = sizeof(uint64_t) * 8;
constexpr size_t bucketsNumber = maxWeightNumberBits + 1;

class RadixHeap {
    size_t nodesInBucketsNumber;
    std::array<std::vector<TmpDist>, bucketsNumber> buckets;
    std::array<int64_t, bucketsNumber> bucketMinDistances;
    int64_t minKey;

public:
    RadixHeap(): 
        nodesInBucketsNumber{0},
        buckets{},
        bucketMinDistances{},
        minKey{std::numeric_limits<int64_t>::max()}
    {
        bucketMinDistances.fill(std::numeric_limits<int64_t>::max());
    }

    bool isEmpty() const { return nodesInBucketsNumber == 0; } 

    void push(size_t v, int64_t d) {
        std::size_t bucketIndex;
        if(d == minKey) {
            bucketIndex = 0;
        }
        else {
            bucketIndex = maxWeightNumberBits - __builtin_clz(d ^ minKey);
        }

        buckets[bucketIndex].push_back(TmpDist(d, v));
        nodesInBucketsNumber++;

        if(d < bucketMinDistances[bucketIndex]) {
            bucketMinDistances[bucketIndex] = d;
        }
    }

    void pullNodes() {
        if(!buckets[0].empty())
            return;

        std::size_t bucketIndex = 1;
        while(buckets[bucketIndex].empty()) 
            bucketIndex++;

        minKey = bucketMinDistances[bucketIndex];

        for(const auto& node : buckets[bucketIndex]) {
            std::size_t newBucketIndex;
            if(node.first == minKey) {
                newBucketIndex = 0;
            }
            else {
                newBucketIndex = maxWeightNumberBits - __builtin_clz(node.first ^ minKey);
            }

            buckets[newBucketIndex].push_back(node);

            if(node.first < bucketMinDistances[newBucketIndex]) {
                bucketMinDistances[newBucketIndex] = node.first;
            }
        }

        buckets[bucketIndex].clear();
        bucketMinDistances[bucketIndex] = std::numeric_limits<int64_t>::max();
    }

    size_t pop() {
        pullNodes();

        TmpDist node = buckets[0].back();
        buckets[0].pop_back();
        nodesInBucketsNumber--;
        
        return node.second;
    }
};

std::vector<int64_t> shortestPathsRadix(const Graph& graph, const size_t source) {
    std::vector<int64_t> shortesPaths(graph.getVerticesNumber(), std::numeric_limits<int64_t>::max());
    std::vector<bool> visited(graph.getVerticesNumber(), false);

    RadixHeap radixHeap;

    shortesPaths[source] = 0;
    radixHeap.push(source, 0);

    while(!radixHeap.isEmpty()) {
        size_t v = radixHeap.pop();

        if(visited[v]) {
            continue;
        }

        visited[v] = true;
        int distance = shortesPaths[v];

        for(const auto& neighbor : graph.getNeighbours(v)) {
            int64_t newDist = distance + neighbor.weight;

            if(shortesPaths[neighbor.vertex] > newDist) {
                shortesPaths[neighbor.vertex] = newDist;
                radixHeap.push(neighbor.vertex, newDist);
            }
        }
    }

    return shortesPaths;
}

int main(int argc, char* argv[]) {
    int status = run(argc, argv, "radix", shortestPathsRadix);

    return status;
}