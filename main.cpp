#include "room/Map.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    Map dungeon(5);          // create 5x5 map
    string command;

    cout << "=== DUNGEON CRAWLER (Map Demo) ===\n";
    cout << "Commands: up / down / left / right (or u/d/l/r). Type 'quit' to exit.\n\n";

    while (true) {
        dungeon.printMap();
        cout << "\nYour move: ";
        cin >> command;

        if (command == "quit" || command == "q") {
            cout << "Goodbye!\n";
            break;
        }

        bool success = dungeon.movePlayer(command);
        if (!success) {
            cout << "Please try again.\n";
        }
        cout << endl;
    }

    return 0;
}
