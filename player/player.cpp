#include "player.h"
#include "../healings/healing.h"
#include "../weapons/weapon.h"
#include <algorithm> // we should sort the healing inventory
#include <iostream>
#include <vector>

Player::Player(bool is_hard_difficulty)
    : // constructor
      playerX(0), playerY(0), HP(100), maxHP(100), playerAttack(5),
      key_count(0), kill_count(0),
      weaponCap(is_hard_difficulty ? 2 : 1),   // can change this later
      healingCap(is_hard_difficulty ? 5 : 10), // can change this later
      weaponsInv({new Fist()}) {}

bool Player::pickupWeapon(Weapon* weapon) {
    // no checking same weapon here, someone please implement
    if (weapon->name == "Fist" || weaponsInv.size() >= weaponCap)
        return false;
    else {
        weaponsInv.emplace_back(weapon);
        return true;
    }
}

bool Player::discardWeapon(int index) {
    // do not allow delete fist
    if (index > 0 && index < weaponsInv.size()) {
        std::vector<Weapon*>::iterator it = weaponsInv.begin() + index;
        weaponsInv.erase(it);
        return true;
    }
    return false;
}

int Player::attackEnemy(int index) {
    if (index < 0 || index >= weaponsInv.size())
        index = 0; // use fist if invalid index but someone please validate
    int damage = weaponsInv[index]->useWeapon();
    damage += playerAttack;
    std::cout << "Player deal " << damage << " damage\n";
    return damage;
}

bool Player::pickupHealings(Healing* healing) {
    if (healingsInv.size() >= healingCap) {
        return false;
    } else {
        healingsInv.emplace_back(healing);
        std::sort(healingsInv.begin(), healingsInv.end(),
                  [](Healing*& a, Healing*& b) { return a->name <= b->name; });
        return true;
    }
}

bool Player::discardHealings(int index) {
    if (index >= 0 && index < healingsInv.size()) {
        std::vector<Healing*>::iterator it = healingsInv.begin() + index;
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

void Player::showWeapons(bool showFist) {
    if (showFist) {
        for (int i = 0; i < weaponsInv.size(); i++)
            std::cout << i + 1 << ". " << weaponsInv[i]->shortDescription()
                      << std::endl;
    } else {
        for (int i = 1; i < weaponsInv.size(); i++)
            std::cout << i << ". " << weaponsInv[i]->shortDescription()
                      << std::endl;
    }
}
void Player::showHealings() {
    for (int i = 0; i < healingsInv.size(); i++) {
        std::cout << i + 1 << ". " << healingsInv[i]->name << std::endl;
    }
}
void Player::showStats() {
    std::cout << "HP            : " << HP << '/' << maxHP << std::endl;
    std::cout << "Player attack : " << playerAttack << std::endl;
    std::cout << "Collected keys: " << key_count << "/3" << std::endl;
    std::cout << "Kill count    : " << kill_count << std::endl;
}
void Player::showWeaponDescription(int index) {
    if (index >= 0 && index < weaponsInv.size()) {
        std::cout << weaponsInv[index]->longDescription();
    }
}
void Player::showHealingDescription(int index) {
    if (index >= 0 && index < healingsInv.size()) {
        healingsInv[index]->printDescription();
    }
}
