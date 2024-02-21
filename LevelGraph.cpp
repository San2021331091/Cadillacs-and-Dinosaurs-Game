#include "LevelGraph.h"
#include "Queue.h"

void LevelGraph::addLevel(const string& levelName) {
    adjacencyList[levelName];
}

void LevelGraph::addConnection(const string& fromLevel, const string& toLevel) {
    adjacencyList[fromLevel].push_back(toLevel);
    adjacencyList[toLevel].push_back(fromLevel); 
}

vector<string> LevelGraph::traverseSequentially(const string& startLevel) {
    HashTable<string, bool> visited;
    Queue<string> levelQueue;

    levelQueue.enqueue(startLevel);
    visited[startLevel] = true;
    vector<string> visitedLevels;

    while (!levelQueue.isEmpty()) {
        string currentLevel = levelQueue.getFront();
        levelQueue.dequeue();
       
        visitedLevels.push_back(currentLevel);

        for (const auto& adjacentLevel : adjacencyList[currentLevel]) {
            if (!visited[adjacentLevel]) {
                levelQueue.enqueue(adjacentLevel);
                visited[adjacentLevel] = true;
            }
        }
    }
    return visitedLevels;
}


