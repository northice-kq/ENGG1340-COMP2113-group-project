#include "Map.h"
#include <iostream>
#include <cstdlib>   // for system("clear") if you want; optional

using namespace std;

// Constructor
Map::Map(int gridSize) : size(gridSize), playerX(0), playerY(0) {
    // Create a 2D vector of bool, all false initially
    explored.resize(size, vector<bool>(size, false));
    // Mark starting cell (0,0) as explored
    explored[playerY][playerX] = true;
}

// Destructor – vector cleans itself, but included for completeness
Map::~Map() {
    // nothing to manually free (vector handles it)
}

// Display the map
void Map::printMap() const {
    // optional: clear screen for a fresh view (uncomment if desired)
    // system("clear");   // for Linux/macOS; use "cls" on Windows

    // Print column numbers
    cout << "   ";
    for (int i = 0; i < size; i++) {
        cout << " " << i << "  ";
    }
    cout << endl;

    // Print each row
    for (int y = 0; y < size; y++) {
        cout << " " << y << " ";
        for (int x = 0; x < size; x++) {
            if (playerX == x && playerY == y) {
                cout << "[@] ";
            }
            else if (explored[y][x]) {
                cout << "[.] ";
            }
            else {
                cout << "[?] ";
            }
        }
        cout << endl;
    }
}

// Move the player. Direction: "up", "down", "left", "right"
bool Map::movePlayer(string direction) {
    int newX = playerX;
    int newY = playerY;

    if (direction == "up" || direction == "Up" || direction == "UP" || direction == "u") {
        newY--;
    }
    else if (direction == "down" || direction == "Down" || direction == "DOWN" || direction == "d") {
        newY++;
    }
    else if (direction == "left" || direction == "Left" || direction == "LEFT" || direction == "l") {
        newX--;
    }
    else if (direction == "right" || direction == "Right" || direction == "RIGHT" || direction == "r") {
        newX++;
    }
    else {
        cout << "Invalid command! Use: up/down/left/right (or u/d/l/r).\n";
        return false;
    }

    // Check boundaries
    if (newX < 0 || newX >= size || newY < 0 || newY >= size) {
        cout << "You cannot move there – it's outside the dungeon!\n";
        return false;
    }

    // Valid move: update position and mark new cell as explored
    playerX = newX;
    playerY = newY;
    explored[playerY][playerX] = true;
    return true;
}

// Placeholder – you can expand later when you add an exit condition
bool Map::isGameOver() const {
    // For now, game never ends. Later: if player reaches a special tile
    return false;
}