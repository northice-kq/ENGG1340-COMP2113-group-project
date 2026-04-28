#include "room/Map.h"
#include "output_text/output_text.h"

#include <string>
#include <thread>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

int main() {
    srand(time(0));

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
    cin >> diff_choice;
    bool is_hard = (diff_choice == 2);


    Map dungeon(is_hard);          // create map
    Player player(is_hard);
    int size = dungeon.getSize();
    vector<vector<Room>> room_grid = assign_rooms(size); // assigning the rooms
    this_thread::sleep_for(chrono::milliseconds(1500));

    bool game_running = true;
    bool player_won = false;

    // game intro, play BEFORE map is shown
    Room& start_room = room_grid[0][0];
    enter_start_room(start_room, player);

   // game loop starts
     while (game_running) {
         dungeon.printMap();
         player.showStats();

         cout << "\nCommands: up / down / left / right (or u/d/l/r). Type 'quit' to exit.\n\n";
         string command;

         while (true) {
             writer_print("Your move: ", false, false);
             cin >> command;

             // quit
             if (command == "quit" || command == "q" || command == "Quit") {
                 writer_print("You flee the dungeon. Coward");
                 game_running = false;
                 break;
             }

             // valid input
             if (command == "up" || command == "u" || command == "Up" || command == "UP" ||
                 command == "down" || command == "d" || command == "Down" || command == "DOWN" ||
                 command == "left" || command == "l" || command == "Left" || command == "LEFT" ||
                 command == "right" || command == "r" || command == "Right" || command == "RIGHT") {

             // if it fails (out of bounds), ask again
             if (dungeon.movePlayer(command)) {
                 break; // success
             }
             // movePlayer already printed the error message
                continue; //ask for input again
            }
             // Invalid — clear and retry without reprinting map
             writer_print("Invalid command! Use: up/down/left/right (or u/d/l/r), quit (q)",false);
             cin.clear();
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
         }

         if (!game_running) break; // player quit

         // get current room
          int px = dungeon.getPlayer().playerX;
          int py = dungeon.getPlayer().playerY;
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
                             dungeon.setEscapeRoom(x, y);
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

        // check dropped items after any room entry (except mirror warning rooms)
         if (game_running && !current_room.has_warning) {
             if (!current_room.dropped_weapons.empty() || !current_room.dropped_healings.empty()) {
                 check_room_for_items(current_room, player);
             }
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



    return 0;
}
