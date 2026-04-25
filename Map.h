#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

using namespace std;

class Map {
private:
    int size;                       // grid size (5)
    vector<vector<bool>> explored;  // track visited cells
    int playerX, playerY;           // current position

public:
    Map(int gridSize = 5);          // constructor, creates 5x5 grid
    ~Map();                         // destructor (not strictly needed but good habit)

    void printMap() const;          // display the grid with @ . ?
    bool movePlayer(string direction); // returns true if move valid, false otherwise
    bool isGameOver() const;        // placeholder for later (e.g., exit found)
    int getPlayerX() const { return playerX; }
    int getPlayerY() const { return playerY; }
};

#endif