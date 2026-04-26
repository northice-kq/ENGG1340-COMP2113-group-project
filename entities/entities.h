#ifndef ENTITIES_H
#define ENTITIES_H

#include "../player/player.h"
#include <iostream>
#include <string>

using namespace std;

//parent class of enemy
class Enemy{
public:
    string name;
    int hp;
    int attack;
    
    virtual ~Enemy();
    Enemy(int h, int a, string name);
    
    void takedamage(int damage);
    virtual int attackAction();
    virtual int attackAction(int l);
    virtual bool attemptDodge() {
        return false;
    }
};

//sub-class Mage
class Mage : public Enemy{
public:
    int streak;
    Mage(int h, int att, string name);
    int attackAction(int l) override;
};

//sub-class Mage
class Assassin : public Enemy{
public:
    double sneakRate;
    bool sneaked;
    Assassin(int h, int att, string name);
    int attackAction() override;
    bool attemptDodge() override;
};

Enemy* generateEnemy(int killcnt);

#endif
