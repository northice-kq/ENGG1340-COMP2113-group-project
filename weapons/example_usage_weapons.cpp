#include "weapon.h"
#include <iostream>
#include <vector>
int main() {
    // the inventory must store *pointers* of weapons
    // for example a vector of pointers or a linked list of pointers
    // maybe using linked list can fulfill the dynamic memory requirement
    std::vector<Weapon*> v;
    v.emplace_back(new Fist()); // Fist should be the default weapon and should
                                // not be removed
    std::cout << v[0]->longDescription();
    v[0]->useWeapon();
    std::cout << std::endl;
    v.emplace_back(new CalculatorGun()); // add a new weapon
    std::cout << v[1]->longDescription();
    v[1]->useWeapon();
    v.emplace_back(new Sword(5));
    // add a weapon with custom durability, used for restoring the game
    std::cout << v[2]->longDescription();
}
