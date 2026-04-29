#include "random_event.h"
#include "../player/player.h"
#include "../entities/entities.h"
#include "../output_text/output_text.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    // Seed random
    srand(time(nullptr));
    
    // Create a test player
    Player player(false);  // false = normal difficulty
    player.HP = 100;
    player.maxHP = 100;
    player.playerAttack = 10;
    
    // Create test enemies of each type
    cout << "\n=== TESTING RANDOM EVENTS ===\n";
    
    // Test Standard enemy
    cout << "\n--- Testing with Standard Enemy ---\n";
    Enemy* standardEnemy = new Enemy(50, 15, "Standard");
    bool lootDrop = true;
    bool skipTurn = false;
    
    RandomEvent event = getRandomEvent();
    event.triggerEvent(&player, standardEnemy, lootDrop, skipTurn);
    
    cout << "\nPlayer HP: " << player.HP << "\n";
    cout << "LootDrop: " << (lootDrop ? "true" : "false") << "\n";
    cout << "SkipTurn: " << (skipTurn ? "true" : "false") << "\n";
    
    delete standardEnemy;
    
    // Test trigger chance
    cout << "\n--- Testing Trigger Chance (75% on HARD) ---\n";
    int triggers = 0;
    for (int i = 0; i < 100; i++) {
        if (shouldTriggerRandomEvent(true)) {
            triggers++;
        }
    }
    cout << "Triggered " << triggers << " times out of 100 (expected ~75)\n";
    
    return 0;
}