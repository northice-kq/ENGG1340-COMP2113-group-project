#ifndef PLAYER_H
#define PLAYER_H

#include "../healings/healing.h"
#include "../weapons/weapon.h"
#include <vector>

struct Player {
    int HP;
    int maxHP;
    int attack;
    int key_count;
    int kill_count;
    int weaponCap;
    int healingCap;
    Weapon* current_weapon;
    std::vector<Weapon*> weaponsInv;
    std::vector<Healing*> healingsInv;

    Player(bool is_hard_difficulty);
    bool pickupWeapon(Weapon* weapon);
    bool discardWeapon(int index);
    int attackEnemy();
    bool pickupHealings(Healing* healing);
    bool discardHealings(int index);
    bool useHealing(int index);
    void showWeapons();
    void showHealings();
    void showStats();
};

#endif
