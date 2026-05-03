#ifndef COMBAT_H
#define COMBAT_H

#include "../player/player.h"
#include "../entities/entities.h"

//enter a combat room and start a turn based combat, input the address of player, pointer of enemies generated, and the difficulty of the game
void combatRoom(Player &p1, Enemy* currentEnemy, bool isHard);

#endif
