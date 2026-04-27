#ifndef WEAPON_H
#define WEAPON_H

#include <climits>
#include <string>

struct Weapon {
    std::string name;
    std::string description;
    int baseDamage;
    int durability;
    int critHitRate;
    // if critHitRate = n then have 1/n chance of crit hit
    // -1 means no crit hit is possible
    Weapon(std::string name, std::string description, int baseDamage,
           int durability, int critHitRate);
    std::string shortDescription() const;
    std::string longDescription() const;
    virtual int useWeapon() = 0;
};

struct Fist : Weapon {
    Fist(int durability = INT_MAX);
    int useWeapon() override;
};

struct Sword : Weapon {
    Sword(int durability = 100);
    int useWeapon() override;
};

struct Axe : Weapon {
    Axe(int durabiltiy = 15);
    int useWeapon() override;
};

struct CalculatorGun : Weapon {
    CalculatorGun(int durability = INT_MAX);
    int useWeapon() override;
};

struct Railgun : Weapon {
    Railgun(int durability = INT_MAX);
    int useWeapon() override;
};

#endif
