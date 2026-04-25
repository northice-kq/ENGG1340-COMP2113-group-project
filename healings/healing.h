#ifndef HEALING_H
#define HEALING_H

#include <string>

struct Healing {
    std::string name;
    std::string description;
    int count = 0;
    Healing(std::string name, std::string description);
    void printDescription();
    bool healPlayer(int& HP);
    virtual int getFinalHP(int HP) = 0;
};

struct firstAidKit : Healing {
    firstAidKit();
    virtual int getFinalHP(int HP) override;
};

struct bandage : Healing {
    bandage();
    virtual int getFinalHP(int HP) override;
};

struct medKit : Healing {
    medKit();
    virtual int getFinalHP(int HP) override;
};

#endif
