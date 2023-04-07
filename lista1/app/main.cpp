#include <iostream>
#include <fstream>
#include <string.h>

#include "Graph.h"

void exercise1(Graph& graph, std::ofstream& outputFile) {
    graph.sortVerticesInAdjacencyLists();

    std::cout << "DFS: ";
    Graph dfsSearchTree = graph.DFS(1, std::cout);

    std::cout << "BFS: ";
    Graph bfsSearchTree = graph.BFS(1, std::cout);

    graph.saveWithNetworkXFormat(outputFile);
    outputFile << "\n";
    dfsSearchTree.saveWithNetworkXFormat(outputFile);
    outputFile << "\n";
    bfsSearchTree.saveWithNetworkXFormat(outputFile);
}

void exercise2(Graph& graph) {
    std::vector<unsigned int> topologocalOrder;
    bool isDAG = graph.topologicalSort(topologocalOrder);

    if(isDAG) {
        if(graph.getVerticesNumber() <= 200) {
            std::cout << "Given Graph Is DAG\nTopological Order: ";
            for(unsigned int& v : topologocalOrder) {
                std::cout << v << " ";
            }
            std::cout << "\n";
        }
        else {
            std::cout << "Given Graph Is DAG\nToo Many Vertices To Write Out Topological Order\n";
        }
    }
    else {
        std::cout << "Given Graph Is Not DAG\n";
    }
}

void exercise3(Graph& graph, std::ofstream& outputFile) {
    std::vector<std::vector<unsigned int>> components = graph.getStronglyConnectedComponents();
    std::cout << "Components Number: " << components.size() << std::endl;
    
    if(graph.getVerticesNumber() <= 200) {
        for(unsigned int i = 0; i < components.size(); i++) {
            std::cout << i + 1 << ": " << components[i].size() << "  = ";
            for(unsigned int& v: components[i]) {
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }

        for(unsigned int i = 0; i < components.size(); i++) {
            outputFile << i + 1 << " ";
            for(unsigned int& v: components[i]) {
                outputFile << v << " ";
            }
            outputFile << std::endl;
        }
    }
    else {
        for(unsigned int i = 0; i < components.size(); i++) {
            std::cout << i + 1 << ": " << components[i].size() << std::endl;
        }
    }
}

void exercise4(Graph& graph, std::ofstream& outputFile) {
    std::vector<int> verticesDivision; 
    bool isBipartite = graph.isBipartite(verticesDivision);

    if(isBipartite) {
        std::cout << "Graph Is Bipartie\n";

        if(graph.getVerticesNumber() <= 200) {
            std::cout << "White Vertices: ";
            for(unsigned int i = 1; i < verticesDivision.size(); i++) {
                if(verticesDivision[i] == 1) {
                    std::cout << i << " ";
                }
            }
            std::cout << std::endl;
            std::cout << "Black Vertices: ";
            for(unsigned int i = 1; i < verticesDivision.size(); i++) {
                if(verticesDivision[i] == -1) {
                    std::cout << i << " ";
                }
            }
            std::cout << std::endl;
        }

        outputFile << "B\n";
        for(unsigned int i = 1; i <= verticesDivision.size(); i++) {
            outputFile << verticesDivision[i] << std::endl;
        }
    }
    else {
        std::cout << "Graph Is Not Bipartie\n";

        outputFile << "N\n";
    }
}


int main(int argc, char** argv) {
    if(argc < 3) {
        return 1;
    }

    std::ifstream inputFile{argv[2]};
    Graph graph;
    inputFile >> graph;
    inputFile.close();

    if(strcmp(argv[1], "1") == 0) {
        if(argc < 4) {
            return 1;
        }

        std::ofstream outputFile{argv[3]};
        exercise1(graph, outputFile);
        outputFile.close();
    }
    else if(strcmp(argv[1], "2") == 0) {
        exercise2(graph);
    }
    else if(strcmp(argv[1], "3") == 0) {
        if(argc < 4) {
            return 1;
        }

        std::ofstream outputFile{argv[3]};
        exercise3(graph, outputFile);
        outputFile.close();
    }
    else if(strcmp(argv[1], "4") == 0) {
        if(argc < 4) {
            return 1;
        }

        std::ofstream outputFile{argv[3]};
        exercise4(graph, outputFile);
        outputFile.close();
    }
    else {
        return 1;
    }

    return 0;
}