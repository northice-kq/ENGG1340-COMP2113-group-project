#ifndef PLAYER_H
#define PLAYER_H

#include "../healings/healing.h"
#include "../weapons/weapon.h"
#include <vector>

struct Player {
    int HP;
    int maxHP;
    int key_count;
    int kill_count;
    int weaponCap;
    int healingCap;
    std::vector<Weapon*> weaponsInv;
    std::vector<Healing*> healingsInv;

    Player(bool is_hard_difficulty);
    bool pickupWeapon(Weapon* weapon);
    bool discardWeapon(int index);
    int attackEnemy(int index);
    bool pickupHealings(Healing* healing);
    bool discardHealings(int index);
    bool useHealing(int index);
    void showWeapons();
    void showHealings();
    void showStats();
    void showWeaponDescription(int index);
    void showHealingDescription(int index);
};

#endif
