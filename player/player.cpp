#include "player.h"
#include "../weapons/weapon.h"
#include "../healings/healing.h"
#include <iostream>
#include <vector>

Player::Player(bool is_hard_difficulty) : // constructor
HP(100), attack(2), key_count(0), 
weaponCap(is_hard_difficulty ? 2 : 1), // can change this later
healingCap(is_hard_difficulty ? 5 : 10) // can change this later
{
    weaponsInv.emplace_back(new Fist());
    current_weapon = weaponsInv[0];
}

void Player::pickupWeapon(Weapon* weapon){
    // no checking same weapon here, someone please implement
    if (weapon->name != "Fist" && weaponsInv.size() < weaponCap){
        weaponsInv.emplace_back(weapon);
    }
}
void Player::discardWeapon(int index){
    // do not allow delete fist
    if (index > 0 && index < weaponsInv.size()){
        std::vector<Weapon*>::iterator it = weaponsInv.begin() + index;
        delete *it;
        weaponsInv.erase(it);
    }
}
int Player::attackEnemy() {
    if (weapons.empty() || current_weapon >= weapons.size()) {
        std::cout << "No valid weapon!\n";
        return 0;
    }

    int damage = weapons[current_weapon]->useWeapon();
    damage += attack;

    std::cout << "Player deal " << damage << " damage\n";
    return damage; 
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
void Player::showWeapons() {
    std::cout << "=== Weapons ===\n";
    for (int i = 0; i < weapons.size(); i++) {
        std::cout << i << ": " << weapons[i]->name << std::endl;
    }
}
void Player::showHealings() {
    std::cout << "=== Healings ===\n";
    for (int i = 0; i < healings.size(); i++) {
        std::cout << i << ": " << healings[i]->name << " (x" << healings[i]->count << ")\n";
    }
}
void Player::showStats() {
    std::cout << "HP: " << HP << "\n";
    std::cout << "Attack: " << attack << "\n";
    std::cout << "Keys: " << key_count << "\n";
}
    
