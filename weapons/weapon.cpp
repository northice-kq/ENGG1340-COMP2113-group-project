#include "weapon.h"
#include <climits>
#include <iostream>
#include <string>

Weapon::Weapon(std::string name, std::string description, int baseDamage,
               int durability, int critHitRate)
    : name(name), description(description), baseDamage(baseDamage),
      durability(durability), critHitRate(critHitRate) {}

void Weapon::printDescription() const {
    std::cout << "Name      : " << name << std::endl;
    if (name != "Fist") {
        std::cout << "Durability: ";
        if (durability == INT_MAX) {
            std::cout << "Infinite" << std::endl;
        } else {
            std::cout << durability << std::endl;
        }
    }
    std::cout << description << std::endl;
}

Fist::Fist()
    : Weapon("Fist", "Punch your enemies when you do not have any weapons!", 10,
             INT_MAX, 20) {}

int Fist::useWeapon() {
    int damage = baseDamage;
    if (rand() % critHitRate == 0) {
        damage *= 2;
        std::cout << "Critical hit!" << std::endl;
    }
    return damage;
}

Sword::Sword()
    : Weapon("Sword", "A durable and reliable weapon!", 20, 100, 20) {}

int Sword::useWeapon() {
    if (durability == 0) {
        std::cout << "Your weapon has no durability!" << std::endl;
        return 0;
    }
    // upgrading a level increases 20% damage
    // reduce 1 damage per 20 use
    int damage = baseDamage;
    damage -= (100 - durability) / 20;
    if (rand() % critHitRate == 0) {
        damage *= 2;
        std::cout << "Critical hit!" << std::endl;
    }
    durability--;
    return damage;
}

Axe::Axe()
    : Weapon("Axe", "A strong weapon which has high chance of a critical hit!",
             30, 15, 5) {}

int Axe::useWeapon() {
    if (durability == 0) {
        std::cout << "Your weapon has no durability!" << std::endl;
        return 0;
    }
    // upgrading a level increases 20% damage
    int damage = baseDamage;
    if (rand() % critHitRate == 0) {
        damage *= 2;
        std::cout << "Critical hit!" << std::endl;
    }
    durability--;
    return damage;
}

CalculatorGun::CalculatorGun()
    : Weapon("Calculator gun", "A gun which shoots out numbers randomly!", 0,
             INT_MAX, -1) {}

int CalculatorGun::useWeapon() {
    // shoots out 5 random numbers from 1 to 8
    int damage = baseDamage;
    std::cout << "Shooted numbers:";
    for (int i = 0; i < 5; i++) {
        int x = rand() % 8 + 1;
        std::cout << ' ' << x;
        damage += x;
    }
    return damage;
}
