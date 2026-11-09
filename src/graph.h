#pragma once 

#include <vector>

using namespace std;

class Graph {
    private: 
        int numV;
        vector<vector<int>> adjList; 

    public: 
        Graph(int v);
        void AddEdge(int v1, int v2);
        vector<int> GreedyColor();
};