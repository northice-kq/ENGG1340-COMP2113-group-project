#ifndef DUNGEON_H
#define DUNGEON_H
#include <iostream>

using namespace std;

enum RoomType { //define the type of room
    EMPTY,      // nothing happens
    ENEMY,      // combat encounter
    TREASURE,   // loot or chest, obtaining tools
    KEY,        // rooms containing a key
    BOSS,       // final boss fight
    ESCAPE      // exit to escape (revealed after collecting all keys)
};

struct Room { // stores the state of a single room info
    RoomType type;       // the kind of room
    bool is_key_room;      // true if this room contains one of the 3 keys
    bool revealed;       // True if the player has alr visited this room
    bool has_key;         // true if the key is still here (false after pickup), so we make sure they dont pick it up again
    int x, y;            // coordinates of this room in the grid

    Room() {
        type = EMPTY;
        is_key_room = false;
        revealed = false;
        has_key = false;
        x = 0;
        y = 0;
    }
};

class Dungeon {
    Room** grid;           // we can use 2D dynamic array of eooms, so we suit the proj requirment
    int size;              // grid dimensions (size x size)
    int keys_placed;        // number of key rooms currently placed
    int keys_to_place = 3;       // total keys needed
    int escape_x, escape_y;  // coordinates of the escape room
    int boss_x, boss_y;      // coordinates of the boss room
    bool escape_revealed;   // see if escape room is visible on map


};


#endif DUNGEON_H