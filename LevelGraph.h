#ifndef CADILLACS_AND_DINOSAURS_GAME_LEVELGRAPH_H
#define CADILLACS_AND_DINOSAURS_GAME_LEVELGRAPH_H

#include "Game.h"

class Level {
    public:
    string name;

};

class LevelGraph {
private:
    unordered_map<string, vector<string>> adjacencyList;

public:
    void addLevel(const string& levelName);
    void addConnection(const string& fromLevel, const string& toLevel);
    vector<string> traverseSequentially(const string& startLevel);
  
};

#endif 
