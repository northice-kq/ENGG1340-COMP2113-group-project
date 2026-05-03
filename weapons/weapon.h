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
    // constructor for weapons
    // set weapon name, description, base damage, durability and critical hite rate
    // based on parameter inputs
    std::string shortDescription() const;
    // helper function to print out the weapon information (name + durability)
    // return the string to be printed
    std::string longDescription() const;
    // helper function to print out more weapon information
    // return the string to be printed
    virtual int useWeapon() = 0; 
    // call this to use the weapon once
    // return the damage dealt by weapon
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
    Axe(int durability = 15);
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
