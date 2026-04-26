#include "combatRoom.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void combatRoom(Player &p1, Enemy* currentEnemy) {
    cout << "\n--- BATTLE START ---" << endl;
    cout << "You encountered an enemy with " << currentEnemy->hp << " HP!" << endl;

    // condition
    while (p1.hp > 0 && currentEnemy->hp > 0) {
        // player turn
        cout << "\n[Your Turn]" << endl;
        cout << "1. Attack  2. Use potion" << endl;
        int choice = 0;
        
        while (choice != 1){
            cin >> choice;
            if (!(cin >> choice)) {
                cout << "Please enter a number." << endl;
                cin.clear(); cin.ignore(1000, '\n');
                continue;
            }
            if (choice == 1) {
                p1.showWeapons();
                int weaponchoice;
                cin >> weaponchoice;
                p1.weaponcnt = weaponchoice;
                p1.attackEnemy();
                if (!(currentEnemy->attemptDodge())){
                    currentEnemy->hp -= p1.attack;
                }
                
            } else if (choice == 2){
                p1.showHealings();
                int healingchoice;
                cin >> healingchoice;
                p1.useHealing(healingchoice);
            }
            else{
                cout << "Invalid input, please try again." << endl;
            }
        }
        

        // check if died
        if (currentEnemy->hp <= 0) {
            cout << "\nVictory! You defeated the enemy." << endl;
            p1.killcnt++;
            return; // end battle
        }

        // enemy turn
        cout << "\n[Enemy's Turn]" << endl;
        if (currentEnemy->name == "Mage"){
            int lanechoice = 0;
            cout << "A mage is attacking you, select a lane from 1 - 3";
            while (lanechoice < 1 || lanechoice > 3){
                cin >> lanechoice;
                if (!(cin >> choice)) {
                    cout << "Please enter a number." << endl;
                    cin.clear(); cin.ignore(1000, '\n');
                    continue;
                }
                if (lanechoice < 1 || lanechoice > 3) cout << "wrong input, try again." << endl;
            }
            p1.hp -= currentEnemy->attackAction(lanechoice);
        }
        else if (currentEnemy->name == "Assassin"){
            p1.hp -= currentEnemy->attackAction();
        }
        else if (currentEnemy->name == "Standard"){
            p1.hp -= currentEnemy->attackAction();
        }
        cout << "Your HP: " << p1.hp << endl;

        // check if player died
        if (p1.hp <= 0) {
            cout << "Game Over... You died in the dungeon." << endl;
        }
    }
}
