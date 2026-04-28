#ifndef MAP_H
#define MAP_H
#include "room.h"
#include "../player/player.h"
#include <vector>
#include <string>

struct Map {
    bool is_hard_difficulty;
    int size;                       // grid size (5)
    std::vector<std::vector<bool>> explored;  // track visited cells
    Player player;           // current position

    // showing escape square - @north_ice
    int escapeX, escapeY;
    bool escapeRevealed;

    Map(bool is_hard_difficulty); // constructor
    Map(std::string filepath);    // restore game from text file (implement later)
    ~Map();                         // destructor (not strictly needed but good habit)

    void printMap() const;          // display the grid with @ . ?
    bool movePlayer(std::string direction); // returns true if move valid, false otherwise
    bool isGameOver() const;        // placeholder for later (e.g., exit found)

    /* linking the escape room with room.cpp
     * return the square of the escape room - @north_ice */
    void setEscapeRoom(int x, int y);
    Player& getPlayer() { return player; }
    int getSize() const { return size; }
};

#endif
