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
    v[0]->printDescription();
    v[0]->useWeapon();
    std::cout << std::endl;
    v.emplace_back(new CalculatorGun()); // add a new weapon
    v[1]->printDescription();
    v[1]->useWeapon();
    v[1]->upgrade(1);
    std::cout << std::endl;
    v[1]->printDescription();
    v[1]->useWeapon();
    v[1]->useWeapon();
}
