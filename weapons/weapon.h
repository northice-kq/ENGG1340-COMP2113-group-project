#ifndef WEAPON_H
#define WEAPON_H

#include <string>

struct Weapon {
    std::string name;
    std::string description;
    int baseDamage;
    int durability;
    int level;
    int critHitRate;
    // if critHitRate = n then have 1/n chance of crit hit
    // -1 means no crit hit is possible
    Weapon(std::string name, std::string description, int baseDamage,
           int durability, int critHitRate);
    void printDescription() const;
    bool upgrade(int n);
    virtual int useWeapon() = 0;
};

struct Fist : Weapon {
    Fist();
    int useWeapon() override;
};

struct Sword : Weapon {
    Sword();
    int useWeapon() override;
};

struct Axe : Weapon {
    Axe();
    int useWeapon() override;
};

struct CalculatorGun : Weapon {
    CalculatorGun();
    int useWeapon() override;
};

struct Railgun : Weapon {
    Railgun();
    int useWeapon() override;
};

#endif
