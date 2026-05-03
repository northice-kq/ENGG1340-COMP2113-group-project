#include "save.h"
#include "../weapons/weapon.h"
#include "../healings/healing.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

/*
 * What it does: Factory function: creates a Weapon object from its name and durability.
 * Inputs: name - weapon type as string ("Fist", "Sword", "Axe", "Calculator gun", "Railgun")
 *         durability - initial durability value (0-100 or INT_MAX)
 * Outputs: Returns a pointer to a dynamically allocated Weapon (caller must delete).
 */
// Helper: weapon name -> new Weapon* (factory)
static Weapon* createWeapon(const string& name, int durability) {
    if (name == "Fist") return new Fist(durability);
    if (name == "Sword") return new Sword(durability);
    if (name == "Axe") return new Axe(durability);
    if (name == "Calculator gun") return new CalculatorGun(durability);
    if (name == "Railgun") return new Railgun(durability);
    return new Fist(); // fallback
}

// Helper: healing name -> new Healing*
static Healing* createHealing(const string& name) {
    if (name == "First aid kit") return new firstAidKit();
    if (name == "Bandage") return new bandage();
    if (name == "Med kit") return new medKit();
    return new bandage(); // fallback
}

/*
 * What it does: Saves the entire game state to a text file.
 * Inputs: filename - name of the file to write to
 *         map - Map object (difficulty, size, explored grid, escape room info)
 *         player - Player object (stats, position, inventory)
 *         room_grid - 2D vector of Room objects (type, flags, dropped items)
 * Outputs: None. On success, prints "Game saved to ..." message.
 *          On failure (cannot open file), prints error to cerr.
 */
void saveGame(const string& filename, const Map& map, const Player& player,
              const vector<vector<Room>>& room_grid) {
    ofstream out(filename);
    if (!out) {
        cerr << "Error: Cannot create save file.\n";
        return;
    }

    // 1. Difficulty and grid size
    out << map.is_hard_difficulty << endl;
    out << map.size << endl;

    // 2. Player position
    out << player.playerX << " " << player.playerY << endl;

    // 3. Player stats
    out << player.HP << endl;
    out << player.maxHP << endl;
    out << player.playerAttack << endl;
    out << player.key_count << endl;
    out << player.kill_count << endl;
    out << player.weaponCap << endl;
    out << player.healingCap << endl;

    // 4. Weapons inventory (skip Fist at index 0? Save all for simplicity)
    out << player.weaponsInv.size() << endl;
    for (auto w : player.weaponsInv) {
        out << w->name << endl;
        out << w->durability << endl;
    }

    // 5. Healings inventory
    out << player.healingsInv.size() << endl;
    for (auto h : player.healingsInv) {
        out << h->name << endl;
    }

    // 6. Explored grid
    for (int y = 0; y < map.size; y++) {
        for (int x = 0; x < map.size; x++) {
            out << map.explored[y][x] << " ";
        }
        out << endl;
    }

    // 7. Escape room info
    out << map.escapeX << " " << map.escapeY << endl;
    out << map.escapeRevealed << endl;

    // 8. Room grid (full state)
    out << map.size << endl; // write size again for verification
    for (int y = 0; y < map.size; y++) {
        for (int x = 0; x < map.size; x++) {
            const Room& r = room_grid[y][x];
            out << (int)r.type << endl;
            out << r.revealed << endl;
            out << r.key_collected << endl;
            out << r.chest_looted << endl;
            out << r.start_revisited << endl;
            out << r.start_event_triggered << endl;
            out << r.has_warning << endl;
            
            // dropped weapons
            out << r.dropped_weapons.size() << endl;
            for (auto w : r.dropped_weapons) {
                out << w->name << endl;
                out << w->durability << endl;
            }
            // dropped healings
            out << r.dropped_healings.size() << endl;
            for (auto h : r.dropped_healings) {
                out << h->name << endl;
            }
        }
    }
    out.close();
    cout << "Game saved to " << filename << endl;
}

/*
 * What it does: Reads a save file and populates a GameSaveData structure.
 * Inputs: filename - name of the file to read
 *         outData - reference to GameSaveData that will hold the loaded data
 * Outputs: Returns true if file was successfully read and data is valid.
 *          Returns false if file cannot be opened or size mismatch occurs.
 */
bool loadGame(const string& filename, GameSaveData& outData) {
    ifstream in(filename);
    if (!in) return false;

    // 1. Difficulty and size
    in >> outData.is_hard;
    in >> outData.size;

    // 2. Player position
    in >> outData.playerX >> outData.playerY;

    // 3. Stats
    in >> outData.HP >> outData.maxHP >> outData.playerAttack;
    in >> outData.key_count >> outData.kill_count;
    in >> outData.weaponCap >> outData.healingCap;

    // 4. Weapons
    size_t wcount;
    in >> wcount;
    outData.weapons.clear();
    for (size_t i = 0; i < wcount; i++) {
        string name; int dur;
        getline(in >> std::ws, name);
        in >> std::ws >> dur;
        outData.weapons.push_back({name, dur});
    }

    // 5. Healings
    size_t hcount;
    in >> hcount;
    outData.healings.clear();
    for (size_t i = 0; i < hcount; i++) {
        string name;
        getline(in >> std::ws, name);
        in >> std::ws;
        outData.healings.push_back(name);
    }

    // 6. Explored grid
    outData.explored.resize(outData.size, vector<bool>(outData.size));
    for (int y = 0; y < outData.size; y++) {
        for (int x = 0; x < outData.size; x++) {
            int val; in >> val;
            outData.explored[y][x] = (val != 0);
        }
    }

    // 7. Escape room
    in >> outData.escapeX >> outData.escapeY;
    in >> outData.escapeRevealed;

    // 8. Room grid
    int savedSize;
    in >> savedSize;
    if (savedSize != outData.size) {
        cerr << "Save file size mismatch!\n";
        return false;
    }
    outData.rooms.resize(outData.size, vector<GameSaveData::RoomSaveData>(outData.size));
    for (int y = 0; y < outData.size; y++) {
        for (int x = 0; x < outData.size; x++) {
            auto& rs = outData.rooms[y][x];
            int typeInt;
            in >> typeInt; rs.type = typeInt;
            in >> rs.revealed;
            in >> rs.key_collected;
            in >> rs.chest_looted;
            in >> rs.start_revisited;
            in >> rs.start_event_triggered;
            in >> rs.has_warning;
            
            size_t dwcount;
            in >> dwcount;
            rs.dropped_weapons.clear();
            for (size_t i=0; i<dwcount; i++) {
                string name; int dur;
                getline(in >> std::ws, name);
                in >> std::ws >> dur;
                rs.dropped_weapons.push_back({name, dur});
            }
            size_t dhcount;
            in >> dhcount;
            rs.dropped_healings.clear();
            for (size_t i=0; i<dhcount; i++) {
                string name;
                getline(in >> std::ws, name);
                in >> std::ws;
                rs.dropped_healings.push_back(name);
            }
        }
    }
    in.close();
    return true;
}

/*
 * What it does: Restores the game objects (Map, Player, room_grid) from loaded data.
 * Inputs: data - GameSaveData filled by loadGame()
 *         map - reference to the Map object to be overwritten with saved data
 *         player - reference to the Player object to be overwritten
 *         room_grid - reference to the 2D room grid to be replaced
 * Outputs: None (all three objects are modified directly).
 *          Previously existing dynamic memory (weapons/healings) is properly deleted.
 */
void restoreGame(GameSaveData& data, Map& map, Player& player,
                 vector<vector<Room>>& room_grid) {
    // Restore Map fields
    map.is_hard_difficulty = data.is_hard;
    map.size = data.size;
    map.explored = data.explored;
    map.player.playerX = data.playerX;
    map.player.playerY = data.playerY;
    map.escapeX = data.escapeX;
    map.escapeY = data.escapeY;
    map.escapeRevealed = data.escapeRevealed;

    // Restore Player object
    player.playerX = data.playerX;
    player.playerY = data.playerY;
    player.HP = data.HP;
    player.maxHP = data.maxHP;
    player.playerAttack = data.playerAttack;
    player.key_count = data.key_count;
    player.kill_count = data.kill_count;
    player.weaponCap = data.weaponCap;
    player.healingCap = data.healingCap;

    // Clear existing inventory and rebuild from saved data
    for (auto w : player.weaponsInv) delete w;
    player.weaponsInv.clear();
    for (auto& wp : data.weapons) {
        player.weaponsInv.push_back(createWeapon(wp.first, wp.second));
    }
    for (auto h : player.healingsInv) delete h;
    player.healingsInv.clear();
    for (auto& hp : data.healings) {
        player.healingsInv.push_back(createHealing(hp));
    }

    // Restore room grid
    room_grid.clear();
    room_grid.resize(data.size, vector<Room>(data.size, Room(NOTHING,0,0)));
    for (int y = 0; y < data.size; y++) {
        for (int x = 0; x < data.size; x++) {
            auto& rs = data.rooms[y][x];
            Room& r = room_grid[y][x];
            r.type = (Room_Type)rs.type;
            r.x = x; r.y = y;
            r.revealed = rs.revealed;
            r.key_collected = rs.key_collected;
            r.chest_looted = rs.chest_looted;
            r.start_revisited = rs.start_revisited;
            r.start_event_triggered = rs.start_event_triggered;
            r.has_warning = rs.has_warning;
            
            r.dropped_weapons.clear();
            for (auto& dw : rs.dropped_weapons) {
                r.dropped_weapons.push_back(createWeapon(dw.first, dw.second));
            }
            r.dropped_healings.clear();
            for (auto& dh : rs.dropped_healings) {
                r.dropped_healings.push_back(createHealing(dh));
            }
        }
    }
}
