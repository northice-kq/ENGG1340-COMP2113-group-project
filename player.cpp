#include "player.h"
#include <iostream>
void Player::attackEnemy() {
    if (weapons.empty() || current_weapon >= weapons.size()) {
    std::cout << "No valid weapon!\n";
    return;
    }
    int damage = weapons[current_weapon]->useWeapon();
    damage += attack
    std::cout << "Player deal " << damage << " damage\n";
}
void Player::useHealing(int index) {
    if (healings.empty()) {
        std::cout << "No healing items!\n";
        return;
    }
    if (index < 0 || index >= healings.size()) return;
    bool success = healings[index]->healPlayer(HP);
    if (success) {
        std::cout << "Player HP is now " << HP << std::endl;
    } else {
    std::cout << "Healing failed.\n";
    }
}