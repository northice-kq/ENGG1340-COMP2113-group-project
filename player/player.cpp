#include "player.h"
#include "../healings/healing.h"
#include "../output_text/output_text.h"
#include "../weapons/weapon.h"
#include <algorithm> // we should sort the healing inventory
#include <sstream>
#include <vector>

Player::Player(bool is_hard_difficulty)
    : // constructor
      playerX(0), playerY(0), HP(100), maxHP(100), playerAttack(5),
      key_count(0), kill_count(0),
      weaponCap(is_hard_difficulty ? 2 : 4),   // can change this later
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
    std::ostringstream oss;
    oss << "Player deal " << damage << " damage";
    writer_print(oss.str(), false);
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
        writer_print("No healing items!", false);
        return false;
    }
    if (index < 0 || index >= healingsInv.size())
        return false;
    std::vector<Healing*>::iterator it = healingsInv.begin() + index;
    bool success = (*it)->healPlayer(HP, maxHP);
    if (success) {
        std::ostringstream oss;
        oss << "Player HP is now " << HP;
        writer_print(oss.str(), false);
        delete *it;
        healingsInv.erase(it);
        return true;
    } else {
        writer_print("Healing failed.", false);
        return false;
    }
}

void Player::showWeapons(bool showFist) {
    std::ostringstream oss;
    if (showFist) {
        for (int i = 0; i < weaponsInv.size(); i++)
            oss << i + 1 << ". " << weaponsInv[i]->shortDescription() << '\n';
    } else {
        for (int i = 1; i < weaponsInv.size(); i++)
            oss << i << ". " << weaponsInv[i]->shortDescription() << '\n';
    }
    writer_print(oss.str(), false, false);
}
void Player::showHealings() {
    std::ostringstream oss;
    for (int i = 0; i < healingsInv.size(); i++)
        oss << i + 1 << ". " << healingsInv[i]->name << '\n';
    writer_print(oss.str(), false, false);
}
void Player::showStats() {
    std::ostringstream oss;
    oss << "HP            : " << HP << '/' << maxHP << '\n';
    oss << "Player attack : " << playerAttack << '\n';
    oss << "Collected keys: " << key_count << "/3\n";
    oss << "Kill count    : " << kill_count << '\n';
    writer_print(oss.str(), false, false);
}
void Player::showWeaponDescription(int index) {
    if (index >= 0 && index < weaponsInv.size()) {
        writer_print(weaponsInv[index]->longDescription(), false, false);
    }
}
void Player::showHealingDescription(int index) {
    if (index >= 0 && index < healingsInv.size()) {
        healingsInv[index]->printDescription();
    }
}
