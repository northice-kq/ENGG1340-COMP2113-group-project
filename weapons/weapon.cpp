#include "weapon.h"
#include <climits>
#include <iostream>
#include <string>

Weapon::Weapon(std::string name, std::string description, int baseDamage,
               int durability, int critHitRate)
    : name(name), description(description), baseDamage(baseDamage),
      durability(durability), level(1), critHitRate(critHitRate) {}

void Weapon::printDescription() const {
    std::cout << "Name      : " << name << std::endl;
    if (name != "Fist") {
        std::cout << "Level     : " << level << std::endl;
        std::cout << "Durability: ";
        if (durability == INT_MAX) {
            std::cout << "Infinite" << std::endl;
        } else {
            std::cout << durability << std::endl;
        }
    }
    std::cout << description << std::endl;
}

bool Weapon::upgrade(int n) {
    if (level >= 5 || name == "Fist") {
        return false; // cannot upgrade
    } else {
        int increase = (n < 5 - level ? n : 5 - level);
        level += increase;
        critHitRate -= increase;
        return true; // successful upgrade
    }
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
    std::cout << "Dealt damage: " << damage << std::endl;
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
    int damage = baseDamage + baseDamage * (level - 1) / 5;
    damage -= (100 - durability) / 20;
    if (rand() % critHitRate == 0) {
        damage *= 2;
        std::cout << "Critical hit!" << std::endl;
    }
    durability--;
    std::cout << "Dealt damage: " << damage << std::endl;
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
    int damage = baseDamage + baseDamage * (level - 1) / 5;
    if (rand() % critHitRate == 0) {
        damage *= 2;
        std::cout << "Critical hit!" << std::endl;
    }
    durability--;
    std::cout << "Dealt damage: " << damage << std::endl;
    return damage;
}

CalculatorGun::CalculatorGun()
    : Weapon("Calculator gun", "A gun which shoots out numbers randomly!", 0,
             INT_MAX, -1) {}

int CalculatorGun::useWeapon() {
    // shoots out 5 random numbers from (0 + level) to (7 + level)
    int damage = baseDamage;
    std::cout << "Shooted numbers:";
    for (int i = 0; i < 5; i++) {
        int x = (rand() % 8 + level);
        std::cout << ' ' << x;
        damage += x;
    }
    std::cout << std::endl << "Dealt damage: " << damage << std::endl;
    return damage;
}
