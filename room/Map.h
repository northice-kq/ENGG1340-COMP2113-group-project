#ifndef MAP_H
#define MAP_H
// this file should be called the game engine
#include "../room/room.h"
#include "../player/player.h"
#include <vector>
#include <string>

struct Map {
    bool is_hard_difficulty;
    int size;                       // grid size (5)
    std::vector<std::vector<Room>> grid;  // track visited cells
    Player player;           // current position

    Map(bool is_hard_difficulty); // constructor
    Map(std::string filepath);    // restore game from text file (implement later)
    ~Map();                         // destructor (not strictly needed but good habit)

    void printMap() const;          // display the grid with @ . ?
    bool movePlayer(std::string direction); // returns true if move valid, false otherwise
    bool isGameOver() const;        // placeholder for later (e.g., exit found)
};

#endif
