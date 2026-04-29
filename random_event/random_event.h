#ifndef RANDOM_EVENT_H
#define RANDOM_EVENT_H

#include "../player/player.h"
#include "../entities/entities.h"

struct RandomEvent {
    std::string eventName;
    std::string description;
    
    RandomEvent(std::string name, std::string desc);
    
    // Returns: true if combat should end, false otherwise
    bool triggerEvent(Player* player, Enemy* currentEnemy, bool& lootDrop, bool& skipPlayerTurn);
};

// Get a random event (equal chance for all 5 events)
RandomEvent getRandomEvent();

// Check if random event occurs (0% normal, 75% hard, once per COMBAT)
bool shouldTriggerRandomEvent(bool isHard);

// Get the round number when the event should trigger (4-6th round)
int getEventTriggerRound();

#endif
