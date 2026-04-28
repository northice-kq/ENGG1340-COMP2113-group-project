#include "healing.h"
#include "../output_text/output_text.h"
#include <string>

Healing::Healing(std::string name, std::string description)
    : name(name), description(description) {}

void Healing::printDescription() const {
    writer_print(name + ": " + description, false);
}

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
    : Healing("First aid kit", "Heals player up to 75HP") {}
int firstAidKit::getFinalHP(int HP, int maxHP) const {
    return (HP >= 75 ? -1 : 75);
}

bandage::bandage() : Healing("Bandage", "Heals player 10HP up to 75HP") {}
int bandage::getFinalHP(int HP, int maxHP) const {
    if (HP >= 75)
        return -1;
    else if (HP + 10 > 75)
        return 75;
    else
        return HP + 10;
}

medKit::medKit() : Healing("Med kit", "Heals player to full HP") {}
int medKit::getFinalHP(int HP, int maxHP) const {
    return (HP == maxHP ? -1 : maxHP);
}
