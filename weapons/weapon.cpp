#include "weapon.h"
#include <climits>
#include <iostream>
#include <string>

Weapon::Weapon(std::string name, std::string description, int baseDamage,
               int durability)
    : name(name), description(description), baseDamage(baseDamage),
      durability(durability), level(1) {}

void Weapon::printDescription() {
    std::cout << "Name      : " << name << std::endl;
    std::cout << "Level     : " << level << std::endl;
    std::cout << "Durability: ";
    if (durability == INT_MAX) {
        std::cout << "Infinite" << std::endl;
    } else {
        std::cout << durability << std::endl;
    }
    std::cout << description << std::endl;
}

bool Weapon::upgrade() {
    if (level >= 5 || name == "Fist") {
        return false;
    } else {
        level++;
        return true;
    }
}

Fist::Fist()
    : Weapon("Fist", "Punch your enemies when you do not have any weapons!", 10,
             INT_MAX) {}

int Fist::useWeapon() { return baseDamage; }

Sword::Sword() : Weapon("Sword", "A durable and reliable weapon!", 20, 100) {}

int Sword::useWeapon() {
    if (durability == 0)
        return 0;
    // upgrading a level increases 20% damage
    // reduce 1 damage per 10 use
    int damage = baseDamage;
    damage += baseDamage * (level - 1) / 5;
    damage -= (100 - durability) / 10;
    durability--;
    return damage;
}
