#ifndef HEALING_H
#define HEALING_H

#include <string>

struct Healing {
    std::string name;
    std::string description;
    Healing(std::string name, std::string description); 
    // constructor of healing
    // initialize the healing name and description based on function input
    bool healPlayer(int& HP, int maxHP);
    // call to heal the player (change the player HP in place)
    // function input: player current HP and maximum HP
    // return whether the operation is successful or not
    virtual int getFinalHP(int HP, int maxHP) const = 0;
    // helper function to get the final HP of a player after healing is used
    // function input: player current HP and maximum HP
    // returns -1 if the healing cannot heal the player further
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
