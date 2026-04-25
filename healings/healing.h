#ifndef HEALING_H
#define HEALING_H

#include <string>

struct Healing {
    std::string name;
    std::string description;
    int count = 0;
    Healing(std::string name, std::string description);
    void printDescription() const;
    bool healPlayer(int& HP);
    virtual int getFinalHP(int HP) const = 0;
};

struct firstAidKit : Healing {
    firstAidKit();
    virtual int getFinalHP(int HP) const override;
};

struct bandage : Healing {
    bandage();
    virtual int getFinalHP(int HP) const override;
};

struct medKit : Healing {
    medKit();
    virtual int getFinalHP(int HP) const override;
};

#endif
