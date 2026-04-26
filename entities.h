#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <string>

using namespace std;

struct player{
    int hp;
    int attack;
    int killcnt;
};

//parent class of enemy
class Enemy{
public:
    int hp;
    int attack;
    
    virtual ~Enemy();
    Enemy(int h, int a);
    
    void takedamage(int damage);
    virtual void attackAction();
};

//sub-class Mage
class Mage : public Enemy{
public:
    int streak;
    Mage(int h, int att);
    void attackAction() override;
};

//sub-class Mage
class Assassin : public Enemy{
public:
    double sneakRate;
    Assassin(int h, int att);
    void attackAction() override;
};

Enemy* generateEnemy(int killcnt);

#endif
