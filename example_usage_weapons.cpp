#include "weapons/weapon.h"
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
    v.emplace_back(new Sword()); // add a new weapon
    v[1]->printDescription();
    std::cout << "using the sword" << std::endl;
    std::cout << "sword damage: " << v[1]->useWeapon() << std::endl;
    std::cout << "after using the sword" << std::endl;
    v[1]->printDescription();
}
