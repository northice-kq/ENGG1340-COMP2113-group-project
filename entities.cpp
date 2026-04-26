#include "entities.h"
#include <cstdlib>
#include <iostream>

using namespace std;


// --- Enemy Implementation ---
Enemy::Enemy(int h, int a) : hp(h), attack(a) {}
Enemy::~Enemy() {}

void Enemy::takedamage(int damage) {
    hp -= damage;
    cout << "Enemy took " << damage << " damage. HP remain: " << hp << endl;
}

void Enemy::attackAction() {
    cout << "Enemy deals " << attack << " damage!" << endl;
}

// --- Mage Implementation ---
void Mage::attackAction(){
    cout << "Mage casts a magic spell! Dealing " << attack << " damage." << endl;
}
Mage::Mage(int h, int att): Enemy(h, att) {}

// --- Assassin Implementation ---
void Assassin::attackAction() {
    cout << "Dealing " << attack << " damage." << endl;
}    
Assassin::Assassin(int h, int a): Enemy(h,a), sneakRate(0.3) {}

//factory
Enemy* generateEnemy(int killcnt){
    int hp = 0,attack = 0;
    if (killcnt < 2){
        hp = rand() % 3 + 6;
        attack = rand() % 2 + 1;
        return new Enemy(hp, attack);
    }
    else {
        hp = rand() % 4 + 10;
        attack = rand() % 2 + 2;
        int roll = rand() % 100;
        if (roll<50) return new Assassin(hp, attack);
        else return new Mage(hp, attack*2);
    }
}
