#include "entities.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void combatRoom(player &p1, Enemy* Enemy);

int main(int argc, const char * argv[]) {
    srand(time_t(0));
    player p1 = {10,2};
    Enemy* Enemy = generateEnemy(p1.killcnt);
    combatRoom(p1, Enemy);
    return 0;
}

void combatRoom(player &p1, Enemy* currentEnemy) {
    cout << "\n--- BATTLE START ---" << endl;
    cout << "You encountered an enemy with " << currentEnemy->hp << " HP!" << endl;

    // condition
    while (p1.hp > 0 && currentEnemy->hp > 0) {
        // player turn
        cout << "\n[Your Turn]" << endl;
        cout << "1. Attack  2. Use potion" << endl;
        int choice;
        cin >> choice;

        if (choice == 1) {
            //player.showWeapons();
            
        } else {
            cout << "You brace yourself for the next hit." << endl;
        }

        // check if died
        if (currentEnemy->hp <= 0) {
            cout << "\nVictory! You defeated the enemy." << endl;
            p1.killcnt++;
            return; // end battle
        }

        // enemy turn
        cout << "\n[Enemy's Turn]" << endl;
        currentEnemy->attackAction();
        
        p1.hp -= currentEnemy->attack;
        cout << "Your HP: " << p1.hp << endl;

        // check if player died
        if (p1.hp <= 0) {
            cout << "Game Over... You died in the dungeon." << endl;
        }
    }
}
