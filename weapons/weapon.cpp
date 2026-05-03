#include "weapon.h"
#include "../output_text/output_text.h"
#include <climits>
#include <sstream>
#include <string>

Weapon::Weapon(std::string name, std::string description, int baseDamage,
               int durability, int critHitRate)
    : name(name), description(description), baseDamage(baseDamage),
      durability(durability), critHitRate(critHitRate) {}

std::string Weapon::shortDescription() const {
    std::ostringstream oss;
    oss << name << " (Durability: ";
    if (durability == INT_MAX)
        oss << "Infinite)";
    else
        oss << durability << ')';
    return oss.str();
}

std::string Weapon::longDescription() const {
    std::ostringstream oss;
    oss << "Name      : " << name << '\n';
    if (name != "Fist") {
        oss << "Durability: ";
        if (durability == INT_MAX) {
            oss << "Infinite\n";
        } else {
            oss << durability << '\n';
        }
    }
    oss << description << '\n';
    return oss.str();
}

Fist::Fist(int durability)
    : Weapon("Fist", "Punch your enemies when you do not have any weapons!", 10,
             durability, 20) {}

int Fist::useWeapon() {
    int damage = baseDamage;
    if (rand() % critHitRate == 0) {
        damage *= 2;
        writer_print("Critical hit!", false);
    }
    return damage;
}

Sword::Sword(int durability)
    : Weapon("Sword", "A durable and reliable weapon!", 20, durability, 20) {}

int Sword::useWeapon() {
    if (durability == 0) {
        writer_print("Your weapon has no durability!", false);
        return 0;
    }
    // upgrading a level increases 20% damage
    // reduce 1 damage per 20 use
    int damage = baseDamage;
    damage -= (100 - durability) / 20;
    if (rand() % critHitRate == 0) {
        damage *= 2;
        writer_print("Critical hit!", false);
    }
    durability--;
    return damage;
}

Axe::Axe(int durability)
    : Weapon("Axe", "A strong weapon which has high chance of a critical hit!",
             30, durability, 5) {}

int Axe::useWeapon() {
    if (durability == 0) {
        writer_print("Your weapon has no durability!", false);
        return 0;
    }
    // upgrading a level increases 20% damage
    int damage = baseDamage;
    if (rand() % critHitRate == 0) {
        damage *= 2;
        writer_print("Critical hit!", false);
    }
    durability--;
    return damage;
}

CalculatorGun::CalculatorGun(int durability)
    : Weapon("Calculator gun", "A gun which shoots out numbers randomly!", 0,
             durability, -1) {}

int CalculatorGun::useWeapon() {
    // shoots out 5 random numbers from 1 to 8
    int damage = baseDamage;
    std::ostringstream oss;
    oss << "Shooted numbers:";
    for (int i = 0; i < 5; i++) {
        int x = rand() % 8 + 1;
        oss << ' ' << x;
        damage += x;
    }
    writer_print(oss.str(), false);
    return damage;
}
