#ifndef PLAYER_H
#define PLAYER_H

#include "../healings/healing.h"
#include "../weapons/weapon.h"
#include <vector>

struct Player {
    int playerX, playerY;
    int HP;
    int maxHP;
    int playerAttack;
    int key_count;
    int kill_count;
    int weaponCap;
    int healingCap;
    std::vector<Weapon*> weaponsInv;
    std::vector<Healing*> healingsInv;

    Player(bool is_hard_difficulty); 
    // constructor of player
    // initialize all the variables above
    // function input: the game difficulty
    bool pickupWeapon(Weapon* weapon); 
    // call to add a weapon to player inventory
    // function input: pointer to weapon to be added
    // return whether the operation is successful or not
    bool discardWeapon(int index);
    // call to discard a weapon from player inventory
    // function input: index of weapon in player inventory
    // return whether the operation is successful or not
    int attackEnemy(int index);
    // call to perform attack using a weapon in player inventory
    // function input: index of weapon in player inventory
    // return final damage dealt by player (player attack + damage dealt by weapon)
    bool pickupHealings(Healing* healing);
    // call to add a healing to player inventory
    // function input: pointer to healing to be added
    // return whether the operation is successful or not
    bool discardHealings(int index);
    // call to discard a healing from player inventory
    // function input: index of healing in player inventory
    // return whether the operation is successful or not
    bool useHealing(int index);
    // call to use a healing in player inventory
    // function input: index of healing in player inventory
    // return whether the operation is successful or not
    void showWeapons(bool showFist);
    // print all weapons information in player inventory
    // set showFist to false to hide player's fist in the output
    void showHealings();
    // print all healings information in player inventory
    void showStats();
    // print important stats of player such as player HP
};

#endif
