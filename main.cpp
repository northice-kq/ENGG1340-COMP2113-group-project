#include "room/Map.h"
#include "room/room.h"
#include "output_text/output_text.h"
#include "save/save.h"          // added for save/load Bryan 20260430

#include <string>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

using namespace std;

int main() {
    srand(time(0));

    // ----- Added: Ask to load saved game ----- Bryan 20260430
    bool loaded = false;
    bool is_hard = false;
    vector<vector<Room>> room_grid;
    Map* dungeon = nullptr;
    Player* player_ptr = nullptr;

    cout << "Load saved game? (y/n): ";
    char loadChoice;
    cin >> loadChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (loadChoice == 'y' || loadChoice == 'Y') {
        GameSaveData saveData;
        if (loadGame("savegame.txt", saveData)) {
            // Create temporary map with the saved difficulty, then restore full state
            dungeon = new Map(saveData.is_hard);
            player_ptr = &(dungeon->player);
            room_grid = assign_rooms(dungeon->size, is_hard);  // temporary, will be overwritten
            restoreGame(saveData, *dungeon, *player_ptr, room_grid);
            is_hard = saveData.is_hard;
            loaded = true;
            cout << "\nGame loaded. Continuing your adventure...\n";
            this_thread::sleep_for(chrono::milliseconds(1500));
        } else {
            cout << "\nNo save file found. Starting new game.\n";
            this_thread::sleep_for(chrono::milliseconds(1500));
        }
    }

    if (!loaded) {
        // ---- Difficulty Selection ----
        cout << "\n========================================" << "\n";
        cout << "        DUNGEON CRAWLER" << "\n";
        cout << "========================================" << "\n";
        writer_print("Welcome to DUNGEON CRAWLER", false);
        writer_print("Please select difficulty:");
        cout << "  1. Easy" << "\n";
        cout << "  2. Hard" << "\n";
        cout << "\nChoice: ";

        int diff_choice;
        while (true) {
            cin >> diff_choice;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (diff_choice == 1 || diff_choice ==2) {
                break;
            }
            writer_print("Invalid command! Please select difficulty");
            writer_print("Invalid choice! Please enter 1 or 2.", false);
            cout << "Choice: ";
        }      
        is_hard = (diff_choice == 2);

        dungeon = new Map(is_hard);          // create map
        player_ptr = &(dungeon->player);
        int size = dungeon->size;
        room_grid = assign_rooms(size, is_hard); // assigning the rooms
        this_thread::sleep_for(chrono::milliseconds(1500));

        // game intro, play BEFORE map is shown
        Room& start_room = room_grid[0][0];
        enter_start_room(start_room, *player_ptr);
    }

    // Now use pointers for the rest of the game
    Map& dungeon_ref = *dungeon;
    Player& player = *player_ptr;
    int size = dungeon_ref.size;

    bool game_running = true;
    bool player_won = false;

    // game intro is already done if we loaded; but if loaded, we should not replay intro.
    // The loaded game already has start room visited, so skip intro.
    // (No extra code needed because enter_start_room would not trigger again due to flags.)

    // game loop starts
    while (game_running) {
        dungeon_ref.printMap();
        player.showStats();

        cout << "\nCommands: up / down / left / right (or u/d/l/r). Type 'save' to save & quit, 'quit' to exit.\n\n";
        string command;

        while (true) {
            writer_print("Your move: ", false, false);
            getline(cin, command);

            // quit
            if (command == "quit" || command == "q" || command == "Quit") {
                writer_print("You flee the dungeon. Coward");
                game_running = false;
                break;
            }

            // ----- Added: save command ----- Bryan 20260430
            if (command == "save" || command == "Save") {
                saveGame("savegame.txt", dungeon_ref, player, room_grid);
                writer_print("Game saved. See you next time!");
                game_running = false;
                break;
            }

            // valid input
            if (command == "up" || command == "u" || command == "Up" || command == "UP" ||
                command == "down" || command == "d" || command == "Down" || command == "DOWN" ||
                command == "left" || command == "l" || command == "Left" || command == "LEFT" ||
                command == "right" || command == "r" || command == "Right" || command == "RIGHT") {

                // if it fails (out of bounds), ask again
                if (dungeon_ref.movePlayer(command)) {
                    break; // success
                }
                // movePlayer already printed the error message
                continue; //ask for input again
            }
            // Invalid — clear and retry without reprinting map
            writer_print("Invalid command! Use: up/down/left/right (or u/d/l/r), save, quit",false);
        }

        if (!game_running) break; // player quit or saved

        // get current room
        int px = dungeon_ref.player.playerX;
        int py = dungeon_ref.player.playerY;
        Room& current_room = room_grid[py][px];

        // handle room
        switch (current_room.type) {
            case START:
                enter_start_room(current_room, player);
                break;

            case NOTHING:
                enter_empty_room(current_room, player);
                break;

            case KEY:
                enter_key_room(current_room, player.key_count, room_grid, size, px, py, player);
                // if escape was just revealed, tell the map
                for (int y = 0; y < size; y++) {
                    for (int x = 0; x < size; x++) {
                        if (room_grid[y][x].type == ESCAPE && !room_grid[y][x].revealed) {
                            dungeon_ref.setEscapeRoom(x, y);
                        }
                    }
                }
                break;

            case CHEST:
                enter_chest_room(current_room, player);
                break;

            case ENEMY: {
                enter_combat_room(current_room, player, is_hard);

                if (player.HP <= 0) {
                    game_running = false;
                }
                break;
            }

            case ESCAPE:
                enter_escape_room(current_room);
                player_won = true;
                game_running = false;
                break;
        }
        cout << "\n";
    }

    // ---- Game Over ----
    if (player_won) {
        scene_break();
        writer_print("    YOU ESCAPED!    ", false);
        scene_break();
    }
    else if (player.HP <= 0) {
        scene_break();
        writer_print("    YOU DIED!    ", false);
        scene_break();
    }

    cout << "\nFinal Stats:\n";
    cout << "  Attack: " << player.playerAttack << "\n";
    cout << "  HP:     " << player.HP << "/" << player.maxHP << "\n";
    cout << "  Keys:   " << player.key_count << "/3" << "\n";
    cout << "  Kills:  " << player.kill_count << "\n";
    cout << "\n";

    delete dungeon;
    return 0;
}
