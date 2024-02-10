#include "LevelGraph.h"


void LevelGraph::addLevel(const string& levelName) {
    adjacencyList[levelName];
}

void LevelGraph::addConnection(const string& fromLevel, const string& toLevel) {
    adjacencyList[fromLevel].push_back(toLevel);
    adjacencyList[toLevel].push_back(fromLevel); 
}

vector<string> LevelGraph::traverseSequentially(const string& startLevel) {
    unordered_map<string, bool> visited;
    queue<string> levelQueue;

    levelQueue.push(startLevel);
    visited[startLevel] = true;
    vector<string> visitedLevels;

    while (!levelQueue.empty()) {
        string currentLevel = levelQueue.front();
        levelQueue.pop();
       
        visitedLevels.push_back(currentLevel);

        for (const auto& adjacentLevel : adjacencyList[currentLevel]) {
            if (!visited[adjacentLevel]) {
                levelQueue.push(adjacentLevel);
                visited[adjacentLevel] = true;
            }
        }
    }
    return visitedLevels;
}


