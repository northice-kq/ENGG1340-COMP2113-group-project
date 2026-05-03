#ifndef ENTITIES_H
#define ENTITIES_H

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
    //constructor of a standard enemy, input are hp, attack, type of enemy
    Enemy(int h, int a, string name);
    
    void takedamage(int damage);
    void printEnemyDescription();
    virtual int attackAction(int l);
    virtual bool attemptDodge() {
        return false;
    }
};

//sub-class Mage
class Mage : public Enemy{
public:
    int streak;
    //constructor of mage, input are hp, attack, type of enemy
    Mage(int h, int att, string name);
    int attackAction(int l) override;
};

//sub-class Mage
class Assassin : public Enemy{
public:
    double sneakRate;
    bool sneaked;
    //constructor of assassin, input are hp, attack, type of enemy
    Assassin(int h, int att, string name);
    int attackAction(int l) override;
    bool attemptDodge() override;
};

//generate enemy based on difficulty and kill count
Enemy* generateEnemy(int killcnt, bool isHard);

#endif
