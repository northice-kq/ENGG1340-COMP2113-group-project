#include "healing.h"
#include <iostream>
#include <string>

Healing::Healing(std::string name, std::string description)
    : name(name), description(description) {}

void Healing::printDescription() {
    if (count == 0)
        return;
    std::cout << name << ": " << description << std::endl;
    std::cout << "You have: " << count << std::endl;
}

bool Healing::healPlayer(int& HP) {
    if (count == 0) {
        std::string temp = name;
        temp[0] = temp[0] + 'a' - 'A';
        std::cout << "You don't have any " << temp << '!' << std::endl;
        return false;
    }
    int finalHP = getFinalHP(HP);
    if (finalHP == -1) {
        std::cout << name << " cannot heal you further!" << std::endl;
        return false;
    } else {
        HP = finalHP;
        count--;
        return true;
    }
}

firstAidKit::firstAidKit()
    : Healing("First aid kit", "Heals player up to 75HP") {}
int firstAidKit::getFinalHP(int HP) { return (HP >= 75 ? -1 : 75); }

bandage::bandage() : Healing("Bandage", "Heals player 10HP up to 75HP") {}
int bandage::getFinalHP(int HP) {
    if (HP >= 75)
        return -1;
    else if (HP + 10 > 75)
        return 75;
    else
        return HP + 10;
}

medKit::medKit() : Healing("Med kit", "Heals player to full HP") {}
int medKit::getFinalHP(int HP) { return (HP >= 100 ? -1 : 100); }
