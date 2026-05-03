#include "entities.h"
#include "../output_text/output_text.h"
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;


// --- Enemy Implementation ---
Enemy::Enemy(int h, int a, string n) : hp(h), attack(a), name(n) {}
Enemy::~Enemy() {}

void Enemy::takedamage(int damage) {
    hp -= damage;

    //allow slower output to let player has enough time to read the damage, and now consistent with player damage output - @north_ice
    ostringstream oss;
    oss << "Enemy took " << damage << " damage. HP remain: " << hp;
    writer_print(oss.str(), false);
}

int Enemy::attackAction(int l) {
    ostringstream oss; // same as above, line 18 - 21 - @north_ice
    oss << "Enemy deals " << attack << " damage!";
    writer_print(oss.str(), false);
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
    ostringstream oss;
    lane = rand() % 3 + 1;
    if (l == lane){
        int newatt;
        newatt = attack * (streak * 0.5 + 1);
        streak ++;

        // same as above, line 18 - 21 - @north_ice
        oss << "Mage casts a magic spell! Dealing " << newatt << " damage."; 
        writer_print(oss.str(), false);
        return newatt;
    }
    else{
        writer_print("You dodged the attack from the mage!"); // same as above, line 18 - 21 - @north_ice
        streak = 0;
        return 0;
    }
}
Mage::Mage(int h, int att, string n): Enemy(h, att, n) , streak(0) {}

// --- Assassin Implementation ---
int Assassin::attackAction(int l) {
    ostringstream oss;
    if (sneaked){
        int newatt = attack * 1.5;
        oss << "Dealing " << newatt << " damage.";
        // same as above, line 18 - 21 - @north_ice
        writer_print(oss.str(), false);
        sneaked = false;
        return newatt;
    }
    else{
        oss << "Dealing " << attack << " damage.";
        // same as above, line 18 - 21 - @north_ice
        writer_print(oss.str(), false);
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
