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
    //enemy will deduct its hp by inputted integer, which is damage
    void takedamage(int damage);
    //print the description of the enemy
    void printEnemyDescription();
    //return an integer, which is the damage to be deal to the player
    //input number only will be used when the enemy type is mage
    virtual int attackAction(int l);
    //return whether dodge succeed, only apply to assassin, otherwise, the dodge will fail
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
