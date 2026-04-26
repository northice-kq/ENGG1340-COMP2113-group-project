#include "player.h"
#include "../healings/healing.h"
#include "../weapons/weapon.h"
#include <algorithm> // we should sort the healing inventory
#include <iostream>
#include <vector>

Player::Player(bool is_hard_difficulty)
    : // constructor
      HP(100), maxHP(100), attack(2), key_count(0), kill_count(0),
      weaponCap(is_hard_difficulty ? 2 : 1),  // can change this later
      healingCap(is_hard_difficulty ? 5 : 10) // can change this later
{
    weaponsInv.emplace_back(new Fist());
    current_weapon = weaponsInv[0];
}

bool Player::pickupWeapon(Weapon* weapon) {
    // no checking same weapon here, someone please implement
    if (weapon->name == "Fist") {
        std::cout << "Cannot add weapon: " << weapon->name << std::endl;
        return false;
    } else if (weaponsInv.size() >= weaponCap) {
        std::cout << "You cannot pick up more weapons!" << std::endl;
        return false;
    } else {
        weaponsInv.emplace_back(weapon);
        return true;
    }
}

bool Player::discardWeapon(int index) {
    // do not allow delete fist
    if (index > 0 && index < weaponsInv.size()) {
        std::vector<Weapon*>::iterator it = weaponsInv.begin() + index;
        delete *it;
        weaponsInv.erase(it);
        return true;
    }
    return false;
}

int Player::attackEnemy() {
    int damage = current_weapon->useWeapon();
    damage += attack;
    std::cout << "Player deal " << damage << " damage\n";
    return damage;
}

bool Player::pickupHealings(Healing* healing) {
    if (healingsInv.size() >= healingCap) {
        std::cout << "You cannot pick up more healings!" << std::endl;
        return false;
    } else {
        healingsInv.emplace_back(healing);
        std::sort(healingsInv.begin(), healingsInv.end(),
                  [](const Healing*& a, const Healing*& b) {
                      return a->name <= b->name;
                  });
        return true;
    }
}

bool Player::discardHealings(int index) {
    if (index >= 0 && index < healingsInv.size()) {
        std::vector<Healing*>::iterator it = healingsInv.begin() + index;
        delete *it;
        healingsInv.erase(it);
        return true;
    }
    return false;
}

bool Player::useHealing(int index) {
    if (healingsInv.empty()) {
        std::cout << "No healing items!\n";
        return false;
    }
    if (index < 0 || index >= healingsInv.size())
        return false;
    std::vector<Healing*>::iterator it = healingsInv.begin() + index;
    bool success = (*it)->healPlayer(HP, maxHP);
    if (success) {
        std::cout << "Player HP is now " << HP << std::endl;
        delete *it;
        healingsInv.erase(it);
        return true;
    } else {
        std::cout << "Healing failed.\n";
        return false;
    }
}

void Player::showWeapons() {
    std::cout << "=== Weapons ===\n";
    for (int i = 0; i < weaponsInv.size(); i++) {
        std::cout << i + 1 << ": " << weaponsInv[i]->name << std::endl;
    }
}
void Player::showHealings() {
    std::cout << "=== Healings ===\n";
    for (int i = 0; i < healingsInv.size(); i++) {
        std::cout << i + 1 << ": " << healingsInv[i]->name << std::endl;
    }
}
void Player::showStats() {
    std::cout << "HP: " << HP << "\n";
    std::cout << "Attack: " << attack << "\n";
    std::cout << "Keys: " << key_count << "\n";
}
