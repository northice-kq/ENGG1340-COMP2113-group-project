#include "entities.h"
#include <cstdlib>
#include <iostream>

using namespace std;


// --- Enemy Implementation ---
Enemy::Enemy(int h, int a, string n) : hp(h), attack(a), name(n) {}
Enemy::~Enemy() {}

void Enemy::takedamage(int damage) {
    hp -= damage;
    cout << "Enemy took " << damage << " damage. HP remain: " << hp << endl;
}

int Enemy::attackAction() {
    cout << "Enemy deals " << attack << " damage!" << endl;
    return attack;
}

// --- Mage Implementation ---
int Mage::attackAction(int l){
    int lane;
    lane = rand() % 3 + 1;
    if (l == lane){
        cout << "Mage casts a magic spell! Dealing " << attack << " damage." << endl;
        int newatt;
        newatt = attack * (streak * 0.5 + 1);
        streak ++;
        return newatt;
    }
    else{
        cout << "You dedged the attack from the mage!" << endl;
        streak = 0;
        return 0;
    }
}
Mage::Mage(int h, int att, string n): Enemy(h, att, n) {}

// --- Assassin Implementation ---
int Assassin::attackAction() {
    if (sneaked){
        cout << "Dealing " << attack << " damage." << endl;
        sneaked = false;
        return attack*1.5;
    }
    else{
        cout << "Dealing " << attack << " damage." << endl;
        return attack;
    }
}
Assassin::Assassin(int h, int a, string n): Enemy(h,a,n), sneakRate(0.3), sneaked(false) {}

bool Assassin::attemptDodge() {
    int roll = rand() % 10;
    return (roll < sneakRate);
}

//factory
Enemy* generateEnemy(int killcnt){
    int hp = 0,attack = 0;
    if (killcnt < 2){
        hp = rand() % 3 + 6;
        attack = rand() % 2 + 1;
        return new Enemy(hp, attack, "Standard");
    }
    else {
        hp = rand() % 4 + 10; //10-13
        attack = rand() % 2 + 2; //2-3
        int roll = rand() % 100;
        if (roll<50) return new Assassin(hp, attack, "Assassin");
        else return new Mage(hp, attack*2, "Mage");
    }
}
