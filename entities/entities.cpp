#include "entities.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;


// --- Enemy Implementation ---
Enemy::Enemy(int h, int a, string n) : hp(h), attack(a), name(n) {}
Enemy::~Enemy() {}

void Enemy::takedamage(int damage) {
    hp -= damage;
    cout << "Enemy took " << damage << " damage. HP remain: " << hp << endl;
}

int Enemy::attackAction(int l) {
    cout << "Enemy deals " << attack << " damage!" << endl;
    return attack;
}

void Enemy::printEnemyDescription(){
    cout << "╔═══════════════════╗" << endl;
    cout << "║ " << left << setw(8) << "Type: " << right << setw(9) << name << " ║" << endl;
    cout << "║ " << "Health: " << right << setw(9) << hp << " ║" << endl;
    cout << "║ " << "Attack: " << right << setw(9) << attack << " ║" << endl;
    cout << "╚═══════════════════╝" << endl;
}

// --- Mage Implementation ---
int Mage::attackAction(int l){
    int lane;
    lane = rand() % 3 + 1;
    if (l == lane){
        int newatt;
        newatt = attack * (streak * 0.5 + 1);
        streak ++;
        cout << "Mage casts a magic spell! Dealing " << newatt << " damage." << endl;
        return newatt;
    }
    else{
        cout << "You dodged the attack from the mage!" << endl;
        streak = 0;
        return 0;
    }
}
Mage::Mage(int h, int att, string n): Enemy(h, att, n) {}

// --- Assassin Implementation ---
int Assassin::attackAction(int l) {
    if (sneaked){
        int newatt = attack * 1.5;
        cout << "Dealing " << newatt << " damage." << endl;
        sneaked = false;
        return newatt;
    }
    else{
        cout << "Dealing " << attack << " damage." << endl;
        return attack;
    }
}
Assassin::Assassin(int h, int a, string n): Enemy(h,a,n), sneakRate(0.3), sneaked(false) {}

bool Assassin::attemptDodge() {
    int roll = rand() % 10;
    if (roll < sneakRate*10){
        sneaked = true;
        return true;
    }
    else return false;
}

//factory
Enemy* generateEnemy(int killcnt, bool isHard){
    int hp = 0,attack = 0;
    int roll = rand() % 100;
    double diff = isHard? 1.5 : 1.0;
    hp = diff * (rand() % (11 + 3*killcnt) + (50 + 3*killcnt));
    attack = diff * (rand() % (4 + 2*killcnt) + (10 + 1*killcnt));
    if (roll < 20) return new Enemy(hp, attack, "Standard");
    if (roll < 60) return new Assassin(hp, attack, "Assassin");
    else return new Mage(hp, attack*2, "Mage");
}
