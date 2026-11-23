#include <vector>
#include <algorithm>

#include "graph.h"
#include "limitations.h"

using namespace std;

Graph::Graph(int v) : adjList(v) {
    numV = v;
}

void Graph::AddEdge(int v1, int v2) {
    adjList[v1].push_back(v2);
    adjList[v2].push_back(v1);   
}

vector<int> Graph::GreedyColor() {
    vector<int> results(numV);
    results[0] = 0; 

    for (int i = 1; i < numV; i++) {
        results[i] = -1; 
    }

    vector<bool> availableColors(NUM_TIME_SLOTS);
    vector<int> roomsTaken(NUM_TIME_SLOTS);
    roomsTaken[0]++;

    for (int j = 1; j < numV; j++) {
        fill(availableColors.begin(), availableColors.end(), true);

        for (const auto& adj: adjList[j]) {
            if (results[adj] != -1) {
                availableColors[results[adj]] = false;
            }
        }

        int color; 
        for (color = 0; color < NUM_TIME_SLOTS; color++) {
            if (availableColors[color] && roomsTaken[color] < TOTAL_ROOMS) {
                break; 
            }
        }

        if (color == NUM_TIME_SLOTS) {
            return {};
        }

        results[j] = color; 
        roomsTaken[color]++; 
    }

    return results; 
}

bool Graph::BacktrackingUtil(vector<int>& results, int toColor, vector<int>& roomsTaken) {
    if (toColor == numV) {
        return true;
    }

    for (int color = 0; color < NUM_TIME_SLOTS; color++) {

        bool available = true; 

        for (const auto& adj: adjList[toColor]) {
            if (results[adj] == color) {
                available = false; 
                break; 
            }
        }

        if (available && roomsTaken[color] < TOTAL_ROOMS) {
            results[toColor] = color; 
            roomsTaken[color]++;

            if(BacktrackingUtil(results, toColor + 1, roomsTaken)) {
                return true; 
            }

            results[toColor] = -1; 
            roomsTaken[color]--;
        }
    }

    return false; 
}

vector<int> Graph::BacktrackingColor() {
    vector<int> results(numV);
    for (int i = 0; i < numV; i++) {
        results[i] = -1; 
    }

    vector<int> roomsTaken(NUM_TIME_SLOTS);

    if (BacktrackingUtil(results, 0, roomsTaken) == false) {
        return {}; 
    }

    return results; 
}