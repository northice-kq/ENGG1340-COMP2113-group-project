#include "healing.h"
#include <iostream>
#include <map>
#include <string>
// the usage of healings is same as weapons for consistency
// i.e. also need to store the healings as pointers
// recommend to use map to store them (or array if you remember which is which)
int main() {
    std::map<std::string, Healing*> mp;
    // first add all the types first, which have count 0 by default
    mp["First aid kit"] = new firstAidKit();
    mp["Bandage"] = new bandage();
    mp["Med kit"] = new medKit();
    // suppose player find some healings in the dungeon
    mp["Bandage"]->count += 10;
    mp["First aid kit"]->count += 1;
    // check healings inventory
    for (auto it = mp.begin(); it != mp.end(); it++) {
        it->second
            ->printDescription(); // automatically skip items player don't have
    }
    // suppose player is hurt and want to use healing
    int HP = 50;
    mp["First aid kit"]->healPlayer(HP);
    std::cout << "New HP: " << HP << std::endl;
    mp["Bandage"]->healPlayer(HP); // case when cannot heal further
    mp["Med kit"]->healPlayer(HP); // case when player do not have med kit

    for (auto it = mp.begin(); it != mp.end(); it++) {
        it->second
            ->printDescription(); // automatically skip items player don't have
    }
}
