#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include "../room/Map.h"
#include "../player/player.h"
#include "../room/room.h"
#include <vector>
#include <string>

// All game data needed to save/load
struct GameSaveData {
    bool is_hard;
    int size;
    int playerX, playerY;
    int HP, maxHP, playerAttack;
    int key_count;
    int kill_count;
    int weaponCap, healingCap;
    
    // Inventory (weapons & healings)
    std::vector<std::pair<std::string, int>> weapons;  // name, durability
    std::vector<std::string> healings;                 // name only
    
    // Map exploration
    std::vector<std::vector<bool>> explored;
    
    // Escape room
    int escapeX, escapeY;
    bool escapeRevealed;
    
    // Room grid: for each room store its full state
    struct RoomSaveData {
        int type;           // Room_Type as int
        bool revealed;
        bool key_collected;
        bool chest_looted;
        bool start_revisited;
        bool start_event_triggered;
        bool has_warning;
        std::vector<std::pair<std::string, int>> dropped_weapons; // name, durability
        std::vector<std::string> dropped_healings;                // name
    };
    std::vector<std::vector<RoomSaveData>> rooms;
};

// Save the entire game to a file
void saveGame(const std::string& filename, const Map& map, const Player& player,
              const std::vector<std::vector<Room>>& room_grid);

// Load from file; returns true if success, false otherwise.
// The loaded data is returned via the GameSaveData struct.
bool loadGame(const std::string& filename, GameSaveData& outData);

// Restore the game from loaded data into the current game objects.
// This function modifies the map, player, and room_grid.
void restoreGame(GameSaveData& data, Map& map, Player& player,
                 std::vector<std::vector<Room>>& room_grid);

#endif