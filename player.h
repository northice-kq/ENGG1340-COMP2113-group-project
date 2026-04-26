#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "weapons/weapon.h"
#include "healings/healing.h"

struct Player {
    int HP = 100;
    int attack = 2;
    int key_count = 0;

    std::vector<Weapon*> weapons;
    std::vector<Healing*> healings;

    int current_weapon = 0;

    int attackEnemy();
    void useHealing(int index);
    void showWeapons();
    void showHealings();
    void showStats();
};

#endif
