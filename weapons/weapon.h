#ifndef WEAPON_H
#define WEAPON_H

#include <string>

struct Weapon {
    std::string name;
    std::string description;
    int baseDamage;
    int durability;
    int level;
    Weapon(std::string name, std::string description, int baseDamage,
           int durability);
    void printDescription();
    bool upgrade();
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

#endif
