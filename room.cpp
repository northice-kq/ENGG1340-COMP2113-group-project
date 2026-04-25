#include "room.h"
#include "output_text.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>  // for using shuffle
#include <random>
using namespace std;

Room::Room(Room_Type room_type, int x_co, int y_co) {
    type = room_type;
    x = x_co;
    y = y_co;
    revealed = false;
    key_collected = false;
    chest_looted = false;
    start_revisited = false;
    start_event_triggered = false;

    if (room_type == START) {
        revealed = true;
    }
}

vector<vector<Room>> assign_rooms(int size) {
    int total = size * size;

    // Building the pool and counts per type
    vector<Room_Type> pool;
    pool.reserve(total); // allocate memory upfront so as to avoid reallocaitons
    pool.push_back(START);  // only 1 square is start (0,0)
    pool.insert(pool.end(), 3, KEY);
    pool.insert(pool.end(), 6, CHEST);
    pool.insert(pool.end(), 10, ENEMY);
    pool.insert(pool.end(), total - 1 - 3 - 6 - 10, NOTHING);

    // pool.begin() + 1 — skip index 0 (START) so it stays at position 0, not shuffling
    shuffle(pool.begin() + 1, pool.end(), default_random_engine{random_device{}()}); //gives true randomness

    // Filling grid to obtain resut
    vector<vector<Room>> grid;
    grid.reserve(size);

    auto it = pool.begin();
    for (int y = 0; y < size; y++) {
        vector<Room> row;
        row.reserve(size);
        for (int x = 0; x < size; x++) {
            row.emplace_back(*it, x, y);
            ++it;
        }
        grid.push_back(move(row));
    }

    // grid[y][x] now accesses the Room at row y, column x
    // grid[0][0].type is always START
    return grid;
}

// What it does:
//              On first entry, plays the game intro and sets the tone
//              On first return, triggers a creepy atmospheric event. all later returns are then brief
void enter_start_room(Room& room) {
    scene_break();

    // for first visit (game intro)
    if (!room.start_revisited) {
        room.start_revisited = true;

        writer_print("You wake up on cold, damp stone");
        writer_print("Your head throbs. You don't remember how you got here");
        writer_print("The room is small. A heavy iron door stands to the north. It's locked");
        this_thread::sleep_for(chrono::milliseconds(1500));
        writer_print("Three keyholes are carved into its frame");
        writer_print("You need to find three keys to escape this dungeon");
        this_thread::sleep_for(chrono::milliseconds(1000));
        writer_print("Somewhere in the darkness, something stirs");
        press_enter_to_continue();
        return;
    }

    // revisit with creepy message
    if (!room.start_event_triggered) {
        room.start_event_triggered = true;

        writer_print("You return to the room where you first awoke");
        writer_print("Something is wrong");
        writer_print("Scratches mark the stone floor where there were none before");
        writer_print("They lead from the center of the room... to the door");
        this_thread::sleep_for(chrono::milliseconds(1000));
        writer_print("Something... or someone was dragged. Recently");
        this_thread::sleep_for(chrono::milliseconds(1000));
        writer_print("You are not alone here");
        press_enter_to_continue();
        return;
    }

    // for all later returns
    writer_print("The room is as you left it. Cold and silent");
    writer_print("The scratches are still there. Nothing else has changed");
}

// What it does: When the player enters an empty room, prints a random
//              atmospheric description using the typewriter effect.
//              Small chance (20%) the player finds a moment to rest

void enter_empty_room(Room& room) {
    room.revealed = true;
    scene_break();
    // sentences with atmospheric flavour
    string flavor_texts[] = {
        "Dust swirls in the stale air as you walk into this room",
        "Cobwebs brush against your face. Eww",
        "Water drips somewhere in the darkness",
        "Your footsteps echo through the empty chamber",
        "The room is bare, cold stone on all sides",
        "It is just you alone in the hall... or not?",
        "Nothing but silence and shadows here",
        "Faded desperate scratches mark the walls, long abandoned",
        "Four cold lifeless mannequins stand in each corner. No sign of life here",
        "The mirror in this room shows your reflection.\nYou blink\nHe didn't"
    };

    // Pick a random flavor text
    int index = rand() % 10;
    writer_print(flavor_texts[index]);

    if (index == 9) {
        writer_print("I suggest you to leave this room as fast as possible");
    }
    else {
        // 20% chance: player finds a brief rest spot
        int rest_chance = rand() % 100;
        if (rest_chance < 20) {
            this_thread::sleep_for(chrono::milliseconds(300));
            writer_print("You take a moment to catch your breath");
            // Healing will be handled by the player system
            // This just signals the player found a rest spot
        }
        writer_print("This room is empty. Please move on to the next room");
    }
}

// What it does: When the player enters a key room, checks if the key has already been taken.
//              if not, marks it collected,
//              At 3 keys, prints a special message signaling the escape room is now available and past to func

void enter_key_room(Room& room, int& keys_collected) {
    room.revealed = true;
    scene_break();
    if (room.key_collected) {
        // room is now just an empty room cuz key is taken
        writer_print("You have already taken the key from this room");
        return;
    }

    // Pick up the key
    room.key_collected = true;
    keys_collected++;

    // Pick a random key description
    if (keys_collected <= 2) {
        string key_texts[] = {
            "A rusty iron key hangs from a hook on the wall",
            "You spot a wooden key lie on a wet puddle",
            "A silver key rests in a dusty alcove",
            "You find a blackened key wedged inside a wet crack",
            "A golden key lies on a ancient piano"
        };

        int index = rand() % 5;
        writer_print(key_texts[index]);
    }
    else {
        writer_print("The room sits a small wooden table");
        writer_print("On it rests a single cup of tea. Still warm");
        writer_print("No sign of anyone having been here");
        this_thread::sleep_for(chrono::milliseconds(500));
        scene_break();
        writer_print("Beneath the cup, you notice a small iron key");
        writer_print("It is slightly warm to the touch");
        writer_print("Every instinct tells you to leave it");
        writer_print("You pick it up anyway");
    }

    this_thread::sleep_for(chrono::milliseconds(200));

    // Print progress
    string progress = "Key collected! (" + to_string(keys_collected) + "/3)";
    writer_print(progress);

    // Special message when the player has all 3 keys
    if (keys_collected == 1) {
        writer_print("Two more keys and you may find the way out");
    }
    else if (keys_collected == 2) {
        writer_print("One more key remains hidden in the dungeon");
    }
    else if (keys_collected == 3) {
        scene_break();
        writer_print("All three keys are now in your possession");
        writer_print("A distant grinding of stone echoes through the halls");
        writer_print("Somewhere... a new passage has opened");
    }
}


    //          Handles a chest room encounter. On first visit, rolls for a random reward and applies it to the player
    //          Subsequet visit will display a visited room
//              Loot table: 65% stat boost, 15% weapon, 15% healing, 5% nothing.
//              Stat boosts: 40% +1 ATK, 20% +2 ATK, 30% +5 HP, 10% +10 HP.
// Inputs:  room         - the chest room
//          weapons       - player's weapon vector (new weapons pushed)
//          healings      - player's healing vector (count incremented)
//          player_attack - player's base attack (may be increased)
//          player_hp     - player's current HP (may be healed)
//          player_max_hp - player's max HP (may be increased)
// *using & so that we can modify from the function
//
void enter_chest_room(Room& room, vector<Weapon*>& weapons, vector<Healing*>& healings, int& player_attack, int& player_hp, int& player_max_hp) {
    room.revealed = true;
    scene_break();

    // Already looted
    if (room.chest_looted) {
        writer_print("The chest lies open and empty, its treasure long taken");
        return;
    }

    string chest_openings[] = {
        "A dusty chest sits in the corner of the room",
        "You spot an old wooden chest against the wall",
        "A heavy iron chest rests on the damp floor",
        "A broken burlap bag lies on the floor"
    };
    writer_print(chest_openings[rand() % 4]);
    writer_print("You open it carefully");

    // roll for loot category
    int category_roll = rand() % 100; // 0-99

    // 5% nothing
    if (category_roll < 5) {
        string empty_texts[] = {
            "Just dust and disappointment",
            "Nothing but cobwebs inside. Someone beat you here",
            "WOW!!! Here goes...... nothing",
            "What's in the chest...... NOTHING!"
        };
        writer_print(empty_texts[rand() % 4]);
        room.chest_looted = true;
        press_enter_to_continue();
        return;
    }

    // 15% healing
    if (category_roll < 20) {
        int healing_roll = rand() % 100;

        Healing* found = nullptr;
        if (healing_roll < 40) {
            // First Aid kit (40%)
            for (auto& h : healings) {
                if (h->name == "First aid kit") {
                    found = h;
                    break;
                }
            }
            if (!found) {
                found = new firstAidKit();
                healings.push_back(found);
            }
            writer_print("You found a First Aid Kit!");
            writer_print("Heals you up to 75HP");
        }
        else if (healing_roll < 80) {
            // bandage (40%)
            for (auto& h : healings) {
                if (h->name == "Bandage") {
                    found = h;
                    break;
                }
            }
            if (!found) {
                found = new bandage();
                healings.push_back(found);
            }
            writer_print("You found a Bandage!");
            writer_print("Heals 10HP, up to 75HP");
        }
        else {
            // Med Kit (20%)
            for (auto& h : healings) {
                if (h->name == "Med kit") {
                    found = h;
                    break;
                }
            }
            if (!found) {
                found = new medKit();
                healings.push_back(found);
            }
            writer_print("You found a Med Kit!");
            writer_print("Fully restores your health");
        }
        found->count++;
        room.chest_looted = true;
        press_enter_to_continue();
        return;
    }

    // 15% for weapon
    if (category_roll < 35) {
        // build list of weapons the player does not own yet
        vector<string> available;
        bool has_sword = false, has_axe = false, has_calcgun = false;

        for (auto& w : weapons) {
            if (w->name == "Sword")           has_sword = true;
            if (w->name == "Axe")             has_axe = true;
            if (w->name == "Calculator gun")  has_calcgun = true;
        }

        if (!has_sword)  available.push_back("Sword");
        if (!has_axe)    available.push_back("Axe");
        if (!has_calcgun) available.push_back("Calculator gun");

        // if all weapons already owned, reroll internally to stat boost
        if (available.empty()) {
            writer_print("The chest contains a weapon rack, but you already have everything");
            writer_print("Instead, you find something else of value");
            // fall through to stat boost below
        }
        else {
            // the weighting is Sword 80%, Axe 15%, CalculatorGun 5%
            int total_weight = 0;
            for (auto& name : available) {
                if (name == "Sword")           total_weight += 80;
                if (name == "Axe")             total_weight += 15;
                if (name == "Calculator gun")  total_weight += 5;
            }

            int weapon_roll = rand() % total_weight;
            string chosen;
            int cumulative = 0;

            for (auto& name : available) {
                int weight = 0;
                if (name == "Sword")           weight = 80;
                if (name == "Axe")             weight = 15;
                if (name == "Calculator gun")  weight = 5;

                if (weapon_roll < cumulative + weight) {
                    chosen = name;
                    break;
                }
                cumulative += weight;
            }

            // Add the weapon
            Weapon* new_weapon = nullptr;
            if (chosen == "Sword") {
                new_weapon = new Sword();
                writer_print("You found a Sword!");
                writer_print("A durable and reliable weapon. 20 base damage");
            }
            else if (chosen == "Axe") {
                new_weapon = new Axe();
                writer_print("You found an Axe!");
                writer_print("High crit chance. 30 base damage. Low durability");
            }
            else if (chosen == "Calculator gun") {
                new_weapon = new CalculatorGun();
                writer_print("You found the Calculator Gun!");
                writer_print("Shoots random numbers. Chaotic but powerful");
            }
            if (new_weapon) {
                weapons.push_back(new_weapon);
            }

            room.chest_looted = true;
            press_enter_to_continue();
            return;
        }
    }

    // for the rest 65% or reroll form full weapon obtained
    int stat_roll = rand() % 100; // 0-99
    if (stat_roll < 40) {
        // +1 attack (40%)
        player_attack += 1;
        string texts[] = {
            "You find an old training manual. Your technique improves slightly!",
            "A worn whetstone lets you sharpen your fighting edge",
            "A fighting comic. Better than nothing, I guess"
        };
        writer_print(texts[rand() % 3]);
        writer_print("Attack increased by 1! (" + to_string(player_attack) + ")");
    }
    else if (stat_roll < 60) {
        // +2 Attack (20%)
        player_attack += 2;
        string texts[] = {
            "You discover a master's fighting scroll. Power surges through you",
            "A sticky note that says 'hit harder, dummy.' It worked",
            "You felt a power surge from all the fallen adventurers in this dungeon"
        };
        writer_print(texts[rand() % 3]);
        writer_print("Attack increased by 2! (" + to_string(player_attack) + ")");
    }
    else if (stat_roll < 90) {
        // +5 Max HP (30%)
        player_max_hp += 5;
        player_hp += 5;
        string texts[] = {
            "You drink from a shimmering fountain. You feel sturdier",
            "A warm glow envelops you. Your body feels reinforced",
            "Most humans are soft and weak, but you've got admirable heft!"
        };
        writer_print(texts[rand() % 3]);
        writer_print("Max HP increased by 5! (" + to_string(player_max_hp) + ")");
    }
    else {
        // +10 Max HP (10%)
        player_max_hp += 10;
        player_hp += 10;
        string texts[] = {
            "You find a blessed elixir. Vitality courses through your veins",
            "A divine warmth fills the room. Your body is remade stronger",
            "A molden cheese sandwich! Delicious, nutritious, and absolutely not going to give you food poisoning"
        };
        writer_print(texts[rand() % 3]);
        writer_print("Max HP increased by 10! (" + to_string(player_max_hp) + ")");
        writer_print("Now you can die, but... slower");
    }

    room.chest_looted = true;
    press_enter_to_continue();
}

