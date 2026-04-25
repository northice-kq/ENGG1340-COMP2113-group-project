#ifndef ROOM_H
#define ROOM_H
#include "weapon.h"
#include "healing.h"
#include <vector>
using namespace std;
// What it does: Defines the Room_Type enum and Room struct for the dungeon crawler.
//              Each room has a type, coordinate position, and flags to track

// What it does: Defines the possible types of rooms in the grid
//              START    - player's beginning position, always at (0,0)
//              NOTHING  - empty room
//              KEY      - contains one of the three keys needed to escape
//              CHEST    - contains random loot (potion, weapon, armor)
//              ENEMY    - triggers combat encounter

enum Room_Type {
    START,
    NOTHING,
    KEY,
    CHEST,
    ENEMY
};

// What it does: Represents a single room in the dungeon grid
//              Tracks the room's type, position, exploration status,
//              and whether its contents (keys or chests) have been taken

struct Room {
    Room_Type type;         // what kind of room this is
    int x;                  // column coordinate in the grid
    int y;                  // row coordinate in the grid
    bool revealed;          // true if player has visited this room
    bool key_collected;     // true if the key in this room has been taken
    bool chest_looted;      // true if the chest in this room has been opened
    bool start_revisited;   // true if player goes back to start (0,0)
    bool start_event_triggered;   // true after the creepy return event plays

    // What it does: Constructs a Room with the given type and coordinates.

    // Inputs:  room_type - the type of room (START, NOTHING, KEY, CHEST, ENEMY)
    //          x_co   - column position in the dungeon grid
    //          y_co  - row position in the dungeon grid

    Room(Room_Type room_type, int x_co, int y_co);
};



// what it does: Randomly assigns a Room_Type to every cell in the grid.
//              START at (0,0), then 3 KEY, 6 CHEST, 10 ENEMY, 5 NOTHING
//              using shffule for (x,y)
//              rreturns a 2D vector of Room struct
vector<vector<Room>> assign_rooms(int size);


// What it does: Handles the player entering every different types of rooms
void enter_start_room(Room& room);
void enter_empty_room(Room& room);
void enter_key_room(Room& room, int& keys_collected);
void enter_chest_room(Room& room, vector<Weapon*>& weapons, vector<Healing*>& healings, int& player_attack, int& player_hp, int& player_max_hp);
#endif
