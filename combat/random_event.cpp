#include "random_event.h"
#include "../output_text/output_text.h"
#include "../healings/healing.h"
#include <cstdlib>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

RandomEvent::RandomEvent(string name, string desc)
    : eventName(name), description(desc) {}

bool RandomEvent::triggerEvent(Player* player, Enemy* currentEnemy, bool& lootDrop, bool& skipPlayerTurn) {
    // evnet header
    ostringstream oss;
    oss << "\n[!!!] RANDOM EVENT: " << eventName << " \n";
    writer_print(oss.str(), true);
    this_thread::sleep_for(chrono::milliseconds(500));
    
    // each event
    if (eventName == "[Backfire]") {
        int damage = player->playerAttack;
        player->HP -= damage;
        
        oss.str("");
        oss << "Your attack rebounded! You took " << damage << " damage!";
        writer_print(oss.str(), true);
        
        if (player->HP <= 0) {
            writer_print("You have been defeated by your own attack...", true);
        }
        skipPlayerTurn = false;
        return false;
    }
    else if (eventName == "[Diversion]") {
        int newHP = player->HP * 0.6; // player HP reduced to 60%
        if (newHP < 1) newHP = 1;
        player->HP = newHP;
        
        oss.str("");
        oss << "The bomb explodes! Your HP is reduced to " << player->HP;
        writer_print(oss.str(), true);
        writer_print("The enemy escapes in the confusion!", true);
        
        lootDrop = false;
        skipPlayerTurn = false;
        return true; // combat ends
    }
else if (eventName == "[Lucky find]") {
    writer_print("You spot a hidden treasure pouch!", true);
    // Enemy gets free attack
    if (currentEnemy != nullptr && currentEnemy->hp > 0) {
        int enemyDamage = currentEnemy->attack * 0.5;  // the attack damage is weakened for the free attack
        if (enemyDamage < 1) enemyDamage = 1; //avoids weird damage count lol
        oss.str("");
        oss << "While you got distracted by the pouch, the enemy attacks for " << enemyDamage << " damage!";
        writer_print(oss.str(), true);
        player->HP -= enemyDamage;
    }
        // player gets random healing item
        if (player->healingsInv.size() < player->healingCap) {
            int itemType = rand() % 3; // rand
            Healing* newHealing = nullptr;
            if (itemType == 0) {
                newHealing = new bandage();
                writer_print("You found a Bandage!", true);
            }
            else if (itemType == 1) {
                newHealing = new firstAidKit();
                writer_print("You found a First Aid Kit!", true);
            }
            else {
                newHealing = new medKit();
                writer_print("You found a Med Kit!", true);
            }
            
            if (newHealing) {
                player->pickupHealings(newHealing);
            }
        } else {
            writer_print("Your healing inventory is full! The item is lost.", true);
        }
        
        writer_print("You lose your turn while grabbing the item!", true);
        skipPlayerTurn = true;
        return false;
    }
    else if (eventName == "[Truce?]") {
        writer_print("The enemy lowers their weapon and offers a deal...", true);
        writer_print("[1] Accept (Get healing potion + half XP)", false);
        writer_print("[2] Reject (Enemy becomes enraged)", false);
        cout << "Enter your choice (1 or 2): ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            writer_print("You accept the truce.", true);
            
            if (player->healingsInv.size() < player->healingCap) {
                Healing* potion = new medKit();  // Med kit heals to full
                player->pickupHealings(potion);
                writer_print("You received a Med Kit!", true);
            } else {
                writer_print("Your inventory is full, but you survived!", true);
            }
            
            player->kill_count++; // player gets XP (*** half XP is NOT possible due to int type for kill_count)
            writer_print("You gained experience from the encounter.", true);
            
            lootDrop = false;
            skipPlayerTurn = false;
            return true; // combat ends
        } else {
            writer_print("You reject the deal! The enemy fights with increased rage!", true);
            currentEnemy->attack = currentEnemy->attack * 1.1;
            
            oss.str("");
            oss << "Enemy attack increased to " << currentEnemy->attack << "!";
            writer_print(oss.str(), true);
            
            lootDrop = true;
            skipPlayerTurn = false;
            return false;
        }
    } 
        else if (eventName == "[Truce?]") {
        writer_print("The enemy lowers their weapon and offers a deal...", true);
        writer_print("[1] Accept (Get healing potion + half XP)", false);
        writer_print("[2] Reject (Enemy becomes enraged)", false);
        cout << "Enter your choice (1 or 2): ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            writer_print("You accept the truce.", true);
            

            if (player->healingsInv.size() < player->healingCap) {
                Healing* potion = new medKit();  // Med kit heals to full
                player->pickupHealings(potion);
                writer_print("You received a Med Kit!", true);
            } else {
                writer_print("Your inventory is full, but you survived!", true);
            }
            
            player->kill_count++; // player gets XP
            writer_print("You gained experience from the encounter.", true);
            
            lootDrop = false;
            skipPlayerTurn = false;
            return true; // combat ends
        } else {
            writer_print("You reject the deal! The enemy fights with increased rage!", true);
            currentEnemy->attack = currentEnemy->attack * 1.1; //enemy attack damage ++
            
            oss.str("");
            oss << "Enemy attack increased to " << currentEnemy->attack << "!";
            writer_print(oss.str(), true);
            
            lootDrop = true;
            skipPlayerTurn = false;
            return false;
        }
    }
    else if (eventName == "[Storm's Fury]") {
        float multiplier = 0.6 + (rand() % 21) / 100.0; // 0.6 to 0.8 chance
        
        int oldPlayerHP = player->HP;
        int oldEnemyHP = currentEnemy->hp;
        
        player->HP = player->HP * multiplier;
        if (player->HP < 1) player->HP = 1;
        
        currentEnemy->hp = currentEnemy->hp * multiplier;
        if (currentEnemy->hp < 1) currentEnemy->hp = 1;
        
        oss.str("");
        oss << "Lightning strikes the battlefield!\n"
            << "Your HP: " << oldPlayerHP << " → " << player->HP << "\n"
            << "Enemy HP: " << oldEnemyHP << " → " << currentEnemy->hp;
        writer_print(oss.str(), true);
        
        skipPlayerTurn = false;
        return false;
    }
    
    skipPlayerTurn = false;
    return false;
}

RandomEvent getRandomEvent() {
    // Array of 5 events (equal chance)
    struct EventData {
        string name;
        string description;
    };
    
    EventData events[] = {
        {"[Backfire]", "Your attack rebounded and hit yourself!"},
        {"[Diversion]", "The enemy threw a bomb! The enemy escapes!"},
        {"[Lucky find]", "You spot a hidden supply pouch!"},
        {"[Truce?]", "The enemy offers a deal"},
        {"[Storm's Fury]", "Lightning strikes the battlefield!"}
    };
    
    int index = rand() % 5; 
    return RandomEvent(events[index].name, events[index].description);
}

bool shouldTriggerRandomEvent(bool isHard) {
    if (isHard) {
        // 75% chance to trigger ONCE per combat on HARD mode
        return (rand() % 100) < 75;
    } else {
        // 5% chance to trigger ONCE per combat on NORMAL mode
        return (0);
    }
}

// Decides what round the random_event shall happen in (4-6)
int getEventTriggerRound() {
    return rand() % 3 + 4;  // 4, 5, or 6
}