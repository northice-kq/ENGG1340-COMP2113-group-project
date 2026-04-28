#ifndef HEALING_H
#define HEALING_H

#include <string>

struct Healing {
    std::string name;
    std::string description;
    Healing(std::string name, std::string description);
    bool healPlayer(int& HP, int maxHP);
    virtual int getFinalHP(int HP, int maxHP) const = 0;
};

struct firstAidKit : Healing {
    firstAidKit();
    virtual int getFinalHP(int HP, int maxHP) const override;
};

struct bandage : Healing {
    bandage();
    virtual int getFinalHP(int HP, int maxHP) const override;
};

struct medKit : Healing {
    medKit();
    virtual int getFinalHP(int HP, int maxHP) const override;
};

#endif
