#include "combatRoom.h"
#include "dodge.h"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

void combatRoom(Player &p1, Enemy* currentEnemy, bool isHard) {
    
    //Showing the enemy's stat
    cout << "\n--- BATTLE START ---" << endl;
    cout << "You encountered this enemy..." << endl;
    currentEnemy->printEnemyDescription();
    this_thread::sleep_for(chrono::milliseconds(1500));
    
    //loop body
    while (p1.HP > 0 && currentEnemy->hp > 0){
        
        //part 1: player combat
        cout << "\n[Your Turn]" << endl;
        string choice = "";
        while (choice != "attack"){
            cout << "To attack, type \"attack\" , To heal yourself, type \"heal\", To show both inventories, type \"show\". " << endl;
            cout << "Type your choice here: ";
            cin >> choice;
            if (choice == "attack"){
                p1.showWeapons(true);
                this_thread::sleep_for(chrono::milliseconds(1000));
                cout << "Which weapon do you want to use? Type the number: ";
                int weaponchoice;
                while (!(cin >> weaponchoice) || weaponchoice > p1.weaponsInv.size()) {
                    cout << "Please enter a valid number." << endl;
                    cin.clear(); cin.ignore(1000, '\n');
                    continue;
                }
                cout << endl;
                this_thread::sleep_for(chrono::milliseconds(1000));
                if (!(currentEnemy->attemptDodge())){
                    currentEnemy->hp -= p1.attackEnemy(weaponchoice - 1);
                }
                else{ cout << "The assassin dodged your attack! Attack failed" << endl; }
                cout << "Remaining hp of the enemy: " << ((currentEnemy->hp < 0 )? 0 : currentEnemy->hp) << endl;
                this_thread::sleep_for(chrono::milliseconds(1500));
            }
            else if (choice == "heal"){
                if (p1.healingsInv.size() == 0){
                    cout << "Your healing inventory is empty." << endl;
                    this_thread::sleep_for(chrono::milliseconds(1000));
                    continue;
                }
                p1.showHealings();
                int healingchoice;
                cin >> healingchoice;
                cout << "Which healing tool do you want to use? Type the number: ";
                while (!(cin >> healingchoice) || healingchoice > p1.healingsInv.size()) {
                    cout << "Please enter a valid number." << endl;
                    cin.clear(); cin.ignore(1000, '\n');
                    continue;
                }
                p1.useHealing(healingchoice);
            }
            else if (choice == "show"){
                p1.showWeapons(true);
                p1.showHealings();
                this_thread::sleep_for(chrono::milliseconds(1000));
            }
            else cout << "please input a valid choice" << endl;
        }
        
        
        // check if died
        if (currentEnemy->hp <= 0) {
            cout << "\nVictory! You defeated the enemy." << endl;
            this_thread::sleep_for(chrono::milliseconds(1000));
            cout << "Leveled up! You are getting stronger." << endl;
            this_thread::sleep_for(chrono::milliseconds(1000));
            
            int HPreward = rand() % 6 + 5;
            int Attackreward = rand() % 3 + 3;
            p1.HP += HPreward;
            p1.maxHP += HPreward;
            p1.playerAttack += Attackreward;
            p1.kill_count++;
            
            cout << "Player stat: " << endl;
            cout << left << setw(9) << "Health: " << p1.HP-HPreward << '/' << p1.maxHP-HPreward << "->" << p1.HP << '/' << p1.maxHP << endl;
            cout << left << setw(9) << "Attack: " << p1.playerAttack-Attackreward << "->" << p1.playerAttack << endl;
            cout << "You have killed " << p1.kill_count << " enemies." << endl;
            
            return; // end battle
        }
        
        // enemy turn
        cout << "\n[Enemy's Turn]" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
        int lane = 0;
        if (currentEnemy->name == "Mage"){
            cout << "The mage will attack on of the three lanes, type '1','2', or '3' to dodge the attack! " << endl;
            cout << "Enter your choice: ";
            while (!(cin >> lane) || lane > 3) {
                cout << "Please enter a valid number." << endl;
                cin.clear(); cin.ignore(1000, '\n');
            }
            p1.HP -= currentEnemy->attackAction(lane);
        }
        else {
            int roll = rand() % 100;
            if (roll < (isHard? 30:50)){
                if (!Dodge()) p1.HP -= currentEnemy->attackAction(lane);
                //p1.HP -= currentEnemy->attackAction(lane);
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            else p1.HP -= currentEnemy->attackAction(lane);
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        cout << "Your HP: " << p1.HP << endl;
        this_thread::sleep_for(chrono::milliseconds(1500));
        
        // check if player died
        if (p1.HP <= 0) {
            cout << "Game Over... You died in the dungeon." << endl;
        }
        
        
    }
}
