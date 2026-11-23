#pragma once 

#include <vector>

using namespace std;

class Graph {
    private: 
        int numV;
        vector<vector<int>> adjList; 
        bool BacktrackingUtil(vector<int>& results, int numColored, vector<int>& roomsTaken);

    public: 
        Graph(int v);
        void AddEdge(int v1, int v2);
        vector<int> GreedyColor();
        vector<int> BacktrackingColor();
};