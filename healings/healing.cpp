#include "healing.h"
#include "../output_text/output_text.h"
#include <string>

Healing::Healing(std::string name, std::string description)
    : name(name), description(description) {}

bool Healing::healPlayer(int& HP, int maxHP) {
    int finalHP = getFinalHP(HP, maxHP);
    if (finalHP == -1) {
        writer_print(name + " cannot heal your further!", false);
        return false;
    } else {
        HP = finalHP;
        return true;
    }
}

firstAidKit::firstAidKit()
    : Healing("First aid kit", "Heals player up to 75% of max HP") {}
int firstAidKit::getFinalHP(int HP, int maxHP) const {
    return (HP >= maxHP * 0.75 ? -1 : maxHP * 0.75);
}

bandage::bandage() : Healing("Bandage", "Heals player 20% of the max HP") {}
int bandage::getFinalHP(int HP, int maxHP) const {
    if (HP == maxHP)
        return -1;
    else if (HP + maxHP * 0.2 > maxHP)
        return maxHP;
    else
        return HP + maxHP * 0.2;
}

medKit::medKit() : Healing("Med kit", "Heals player to full HP") {}
int medKit::getFinalHP(int HP, int maxHP) const {
    return (HP == maxHP ? -1 : maxHP);
}
