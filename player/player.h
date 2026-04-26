#ifndef PLAYER_H
#define PLAYER_H

#include "../healings/healing.h"
#include "../weapons/weapon.h"
#include <vector>

struct Player {
    int HP;
    int attack;
    int key_count;
    int weaponCap;
    int healingCap;
    Weapon* current_weapon;
    std::vector<Weapon*> weaponsInv;
    std::vector<Healing*> healingsInv;

    Player(bool is_hard_difficulty);
    void pickupWeapon(Weapon* weapon);
    void discardWeapon(int index);
    int attackEnemy();
    void pickupHealings(Healing* healing);
    void discardHealings(int index);
    void useHealing(int index);
    void showWeapons();
    void showHealings();
    void showStats();
};

#endif
