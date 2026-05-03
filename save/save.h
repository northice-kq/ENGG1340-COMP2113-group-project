#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include "../room/Map.h"
#include "../player/player.h"
#include "../room/room.h"
#include <vector>
#include <string>

/*
 * What it does: Holds all game data that can be saved to or loaded from a file.
 *               Contains player stats, inventory, map exploration, and room grid.
 * Inputs: None (this is a data structure).
 * Outputs: None (used as a container for save/load functions).
 */
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

/*
 * What it does: Writes the current game state to a text file.
 * Inputs: filename - name of the save file
 *         map - the Map object (grid size, exploration, escape room info)
 *         player - the Player object (stats, inventory, keys, etc.)
 *         room_grid - 2D vector of Room objects (all room types and flags)
 * Outputs: None (creates/overwrites a file, prints success/error message)
 */
void saveGame(const std::string& filename, const Map& map, const Player& player,
              const std::vector<std::vector<Room>>& room_grid);

/*
 * What it does: Reads a save file and fills the GameSaveData structure.
 * Inputs: filename - name of the save file to read
 *         outData - reference to GameSaveData to be filled with loaded data
 * Outputs: Returns true if loading succeeded, false otherwise (file missing or corrupted).
 */
bool loadGame(const std::string& filename, GameSaveData& outData);

/*
 * What it does: Restores the game state from loaded data into the current Map,
 *               Player, and room grid (replacing existing content).
 * Inputs: data - the GameSaveData filled by loadGame()
 *         map - reference to the Map object to be restored
 *         player - reference to the Player object to be restored
 *         room_grid - reference to the room grid to be restored
 * Outputs: None (directly modifies map, player, and room_grid).
 */
void restoreGame(GameSaveData& data, Map& map, Player& player,
                 std::vector<std::vector<Room>>& room_grid);

#endif
