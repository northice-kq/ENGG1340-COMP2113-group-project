#include "room.h"
#include "../output_text/output_text.h"
#include "combatRoom.h"
#include "entities.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
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
    has_warning = false;

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
void enter_start_room(Room& room, Player& player) {
    scene_break();

    // for first visit (game intro)
    if (!room.start_revisited) {
        room.start_revisited = true;

        writer_print("You wake up on cold, damp stone");
        writer_print("Your head throbs. You don't remember how you got here");
        writer_print("The room is small. A heavy iron door stands to the north");
        writer_print("It's locked");
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
        writer_print("You'd better find another exit");
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

void enter_empty_room(Room& room, Player& player) {
    room.revealed = true;
    scene_break();

    if (room.has_warning == true) {
        writer_print("You came back. You are not afraid to die, are you?");
        writer_print("The mirror still shows your reflection");
        writer_print("...with blood in its eyes", false);
        writer_print("...smiling", false);
        this_thread::sleep_for(chrono::milliseconds(1000));
        writer_print("Just leave");
        press_enter_to_continue();
        return;
    }

    if (!room.dropped_weapons.empty() || !room.dropped_healings.empty()) {
        check_room_for_items(room, player);
    }
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
        "The mirror in this room shows your reflection"
    };

    // Pick a random flavor text
    int index = rand() % 10;
    writer_print(flavor_texts[index]);

    if (index == 9) {
        room.has_warning = true;  // permanently marks this room
        writer_print("You blink");
        writer_print(("'It' didn't"));
        writer_print("I suggest you to leave this room as fast as possible");
        press_enter_to_continue();
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
        press_enter_to_continue();
    }
}

// What it does: When the player enters a key room, checks if the key has already been taken.
//              if not, marks it collected,
//              At 3 keys, prints a special message signaling the escape room is now available and past to func

void enter_key_room(Room& room, int& keys_collected, vector<vector<Room>>& grid, int size, int player_x, int player_y, Player& player) {
    room.revealed = true;
    scene_break();

    if (!room.dropped_weapons.empty() || !room.dropped_healings.empty()) {
        check_room_for_items(room, player);
    }

    if (room.key_collected) {
        // room is now just an empty room cuz key is taken
        writer_print("You have already taken the key from this room");
        press_enter_to_continue();
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
        reveal_escape_room(grid, size, player_x, player_y);
    }
    press_enter_to_continue();
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
void enter_chest_room(Room& room, Player& player) {
    room.revealed = true;
    scene_break();

    // Already looted
    if (room.chest_looted) {
        writer_print("The chest lies open and empty, its treasure long taken");
        press_enter_to_continue();

        if (!room.dropped_weapons.empty() || !room.dropped_healings.empty()) {
            writer_print("You notice something on the floor...");
            check_room_for_items(room, player);
        }
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
    // 15% healing
    if (category_roll < 20) {
        int healing_roll = rand() % 100;

        Healing* new_healing = nullptr;

        if (healing_roll < 40) {
            new_healing = new firstAidKit(); //40%
            writer_print("You found a First Aid Kit!");
            writer_print("Heals you up to 75HP");
        }
        else if (healing_roll < 80) {
            new_healing = new bandage(); //40%
            writer_print("You found a Bandage!");
            writer_print("Heals 10HP, up to 75HP");
        }
        else {
            new_healing = new medKit(); //20%
            writer_print("You found a Med Kit!");
            writer_print("Fully restores your health");
        }

        bool success = player.pickupHealings(new_healing);
        if (!success) {
            writer_print("But your healing inventory is full!");
            drop_healing_in_room(room, new_healing);
        }

        room.chest_looted = true;
        press_enter_to_continue();
        return;
    }

    // 15% for weapon
    if (category_roll < 35) {
        // Roll weapon type
        int weapon_roll = rand() % 100; // 0-99

        string chosen;
        if (weapon_roll < 40) {
            chosen = "Sword";           //40%
        }
        else if (weapon_roll < 70) {
            chosen = "Axe";             //30%
        }
        else if (weapon_roll < 85) {
            chosen = "Calculator gun";  //15%
        }
        else {
            chosen = "Railgun";         //15%
        }

        // check if player already owns this weapon type
        Weapon* existing = nullptr;
        for (auto& w : player.weaponsInv) {
            if (w->name == chosen) {
                existing = w;
                break;
            }
        }
        // if yes, durability may increase
        if (existing) {
            bool infinite_durability = (existing->name == "Calculator gun" ||
                                        existing->name == "Railgun");

            writer_print("You found another " + chosen + "!");

            if (infinite_durability) {
                // infinite durability weapons grant stat boost directly
                this_thread::sleep_for(chrono::milliseconds(300));
                writer_print("Its power resonates with your existing weapon!");

                if (chosen == "Calculator gun") {
                    player.playerAttack += 2;
                    writer_print("The numbers align perfectly. Attack +2! (" +
                                 to_string(player.playerAttack) + ")");
                }
                else if (chosen == "Railgun") {
                    player.maxHP += 10;
                    player.HP += 10;
                    writer_print("The charge flows through you. Max HP +10! (" +
                                 to_string(player.maxHP) + ")");
                }
            }
            else {
                // restore durability by 10 for sword and axe
                int restored = 10;
                if (existing->durability + restored > 100) {
                    restored = 100 - existing->durability;
                }
                existing->durability += restored;

                if (restored > 0) { // if resstorable
                    writer_print("Durability restored by " + to_string(restored) +
                                 "! (" + to_string(existing->durability) + "/100)");
                }
                else {
                    writer_print("Durability is already full! (" +
                                 to_string(existing->durability) + "/100)");
                }

                // grant stat boost when durability is full
                if (existing->durability == 100) {
                    this_thread::sleep_for(chrono::milliseconds(300));
                    writer_print("The " + chosen + " is in perfect condition!");

                    if (chosen == "Sword") {
                        player.playerAttack += 1;
                        writer_print("Its sharp edge inspires you. Attack +1! (" +
                                     to_string(player.playerAttack) + ")");
                    }
                    else if (chosen == "Axe") {
                        player.maxHP += 5;
                        player.HP += 5;
                        writer_print("Its weight feels reassuring. Max HP +5! (" +
                                     to_string(player.maxHP) + ")");
                    }
                }
            }

            room.chest_looted = true;
            press_enter_to_continue();
            return;
        }

        // add to inventory with new weapon
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
        else if (chosen == "Railgun") {
            new_weapon = new Railgun();
            writer_print("You found the Railgun!");
            writer_print("Charge up for massive damage. Timing is everything");
        }

        if (new_weapon) {
            bool success = player.pickupWeapon(new_weapon);
            if (!success) {
                writer_print("But your weapon inventory is full!");
                writer_print("Consider discarding a weapon");
                drop_weapon_in_room(room, new_weapon);
            }
        }

        room.chest_looted = true;
        press_enter_to_continue();
        return;
    }

    // for the rest 65% or reroll form full weapon obtained
    int stat_roll = rand() % 100; // 0-99
    if (stat_roll < 40) {
        // +1 attack (40%)
        player.playerAttack += 1;
        string texts[] = {
            "You find an old training manual. Your technique improves slightly!",
            "A worn whetstone lets you sharpen your fighting edge",
            "A fighting comic. Better than nothing, I guess"
        };
        writer_print(texts[rand() % 3]);
        writer_print("Attack increased by 1! (" + to_string(player.playerAttack) + ")");
    }
    else if (stat_roll < 60) {
        // +2 Attack (20%)
        player.playerAttack += 2;
        string texts[] = {
            "You discover a master's fighting scroll. Power surges through you",
            "A sticky note that says 'hit harder, dummy.' It worked",
            "You felt a power surge from all the fallen adventurers in this dungeon"
        };
        writer_print(texts[rand() % 3]);
        writer_print("Attack increased by 2! (" + to_string(player.playerAttack) + ")");
    }
    else if (stat_roll < 90) {
        // +5 Max HP (30%)
        player.maxHP += 5;
         player.HP += 5;
        string texts[] = {
            "You drink from a shimmering fountain. You feel sturdier",
            "A warm glow envelops you. Your body feels reinforced",
            "Most humans are soft and weak, but you've got admirable heft!"
        };
        writer_print(texts[rand() % 3]);
        writer_print("Max HP increased by 5! (" + to_string(player.maxHP) + ")");
    }
    else {
        // +10 Max HP (10%)
        player.maxHP += 10;
         player.HP += 10;
        string texts[] = {
            "You find a blessed elixir. Vitality courses through your veins",
            "A divine warmth fills the room. Your body is remade stronger",
            "A molden cheese sandwich! Delicious, nutritious, and absolutely not going to give you food poisoning"
        };
        writer_print(texts[rand() % 3]);
        writer_print("Max HP increased by 10! (" + to_string(player.maxHP) + ")");
        writer_print("Now you can die, but... slower");
    }

    room.chest_looted = true;
    press_enter_to_continue();
}


// What it does: Handles entering a combat room, on first enter, player will fight.
//              on later reentry, narrative text suggesting, something has moved in since the player left.
void enter_combat_room(Room& room, Player& player, bool is_hard) {
    scene_break();

    if (!room.revealed) {
        // combat at the first time
        Enemy* enemy = generateEnemy(player.kill_count, is_hard);
        combatRoom(player, enemy, is_hard);
        delete enemy;
        room.revealed = true;
        press_enter_to_continue();
        return;
    }

    // revisit, narrative about the corpse
    string corpse_texts[] = {
        "The corpse twitches. It shouldn't. You ended it. It twitches again",
        "The remains of the creature have begun to decay. The smell is unbearable",
        "Nothing remains but a fresh bloody liver on the stone floor",
        "The eyes of the dead thing are open. They weren't when you left",
        "The body is still here. You step around it carefully"
    };
    writer_print(corpse_texts[rand() % 5]);

    this_thread::sleep_for(chrono::milliseconds(1000));

    // small chance something useful was left behind, this will be a good random event
    int scavenge_roll = rand() % 100;
    if (scavenge_roll < 5) {
        writer_print("You search the remains");
        this_thread::sleep_for(chrono::milliseconds(400));

        int find_roll = rand() % 100;
        if (find_roll < 50) {
            Healing* found = new bandage();
            writer_print("You find a partially used Bandage");
            bool success = player.pickupHealings(found);
            if (!success) {
                drop_healing_in_room(room, found);
            }
        }
        else {
            player.playerAttack += 1;
            writer_print("Studying the wounds teaches you something");
            writer_print("Attack increased by 1! (" + to_string(player.playerAttack) + ")");
        }
    }

    // check for dropped items as usual
    if (!room.has_warning) {
        if (!room.dropped_weapons.empty() || !room.dropped_healings.empty()) {
            check_room_for_items(room, player);
        }
    }
    press_enter_to_continue();
}

// What it does: generate escape room when a player obtained 3 keys.
//              the square that the player currently on, the starting square, the adjacent squares cannot be the escape room
//              output modifies the grid
void reveal_escape_room(vector<vector<Room>>& grid, int size, int player_x, int player_y) {
    // build list of valid coordinates, exc. start & current & adjacent square
    vector<pair<int, int>> valid;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            // Skip starting sq
            if (x == 0 && y == 0) continue;

            // skip player position
            if (x == player_x && y == player_y) continue;

            // skip adjacent rooms
            if (x == player_x && y == player_y - 1) continue; // up
            if (x == player_x && y == player_y + 1) continue; // down
            if (x == player_x - 1 && y == player_y) continue; // left
            if (x == player_x + 1 && y == player_y) continue; // right

            valid.push_back({x, y});
        }
    }
    // pick a random valid room
    int idx = rand() % valid.size();
    int escape_x = valid[idx].first;
    int escape_y = valid[idx].second;

    // Convert to ESCAPE room
    grid[escape_y][escape_x].type = ESCAPE;
    grid[escape_y][escape_x].revealed = false;   // player must discover it

    // narrativee
    scene_break();
    writer_print("All three keys are now in your possession");
    writer_print("A distant grinding of stone echoes through the halls");
    writer_print("Somewhere... a new passage has opened. The map marked something new");
    writer_print("Find it. Escape while you still can");
}


void enter_escape_room(Room& room) {
    room.revealed = true;
    scene_break();
    writer_print("You push open the heavy door");
    writer_print("Cold fresh air hits your face");
    writer_print("Congratulations. You are free");
}

void drop_weapon_in_room(Room& room, Weapon* weapon) {
    room.dropped_weapons.push_back(weapon);

    string drop_texts[] = {
        "You place the " + weapon->name + " on the cold stone floor",
        "The " + weapon->name + " clatters to the ground",
        "You leave the " + weapon->name + " behind. Perhaps you'll return"
    };
    writer_print(drop_texts[rand() % 3]);
}

void drop_healing_in_room(Room& room, Healing* healing) {
    room.dropped_healings.push_back(healing);
    string drop_texts[] = {
        "You set the " + healing->name + " down gently",
        "The " + healing->name + " joins the dust on the floor",
        "You leave the " + healing->name + " here. It might be useful later"
    };
    writer_print(drop_texts[rand() % 3]);
}

void check_room_for_items(Room& room, Player& player) {
    // disallow leaving items if the room is cursed
    if (room.has_warning) {
        writer_print("The air here feels wrong");
        writer_print("You don't want to leave anything on this floor");
        writer_print("Best to leave. Now");
        return;
    }
    bool items_remain = true;

    while (items_remain && (!room.dropped_weapons.empty() || !room.dropped_healings.empty())) { // ensure there are items on floor

        scene_break();
        writer_print("Shiny tools gleam through your eyes");

        // show weapons on floor
        if (!room.dropped_weapons.empty()) {
            cout << "Weapons on the floor:\n" ;
            for (int i = 0; i < (int)room.dropped_weapons.size(); i++) {
                cout << "    " << i + 1 << ". " << room.dropped_weapons[i]->name << " (Durability: " << room.dropped_weapons[i]->durability << ")" << "\n";
            }
        }

        // show healings on floor
        if (!room.dropped_healings.empty()) {
            cout << "Healings on the floor:\n";
            for (int i = 0; i < (int)room.dropped_healings.size(); i++) {
                cout << "    " << i + 1 << ". " << room.dropped_healings[i]->name << endl;
            }
        }

        cout << "Pick up an item? (w = weapon, h = healing, n = leave): " << flush;
        string choice;
        cin >> choice;

        if (choice == "n" || choice == "N") {
            writer_print("You leave the items where they lie");
            break;
        }

        if (choice == "w" || choice == "W") {
            if (room.dropped_weapons.empty()) {
                writer_print("There are no weapons on the floor");
                continue;
            }

            // show current weapon inventory first
            writer_print("Your weapon inventory:");
            player.showWeapons(true);
            cout << "  Capacity: " << player.weaponsInv.size() << "/" << player.weaponCap + 1 << " (including Fist)" << endl;

            cout << "\n  Which weapon to pick up? (1-" << room.dropped_weapons.size() << ", or 0 to cancel): " << flush;
            int index;
            cin >> index;

            if (index == 0) continue;
            if (index < 1 || index > (int)room.dropped_weapons.size()) {
                writer_print("Invalid choice");
                continue;
            }

            Weapon* to_pickup = room.dropped_weapons[index - 1];
            bool success = player.pickupWeapon(to_pickup); // see if the player have cap, if no, they can't pick up

            if (success) {
                writer_print("You pick up the " + to_pickup->name);
                room.dropped_weapons.erase(room.dropped_weapons.begin() + index - 1);
            }
            else {
                writer_print("Your weapon inventory is full!");
                writer_print("Discard a weapon first, then try again");

                // Offer to discard
                cout << "\n  Discard a weapon? (y/n): " << flush;
                string discard_choice;
                cin >> discard_choice;

                if (discard_choice == "y" || discard_choice == "Y") {
                    player.showWeapons(false);
                    cout << "  Choose weapon to discard (1-" << player.weaponsInv.size() << ", or 0 to cancel): " << flush;
                    int discard_index;
                    cin >> discard_index;

                    if (discard_index > 0 && discard_index <= (int)player.weaponsInv.size()) {
                        Weapon* discarded = player.weaponsInv[discard_index - 1];
                        bool discarded_success = player.discardWeapon(discard_index - 1);

                        if (discarded_success) {
                            writer_print("You discard the " + discarded->name);
                            drop_weapon_in_room(room, discarded);

                            // try pickup again after they discard
                            bool retry = player.pickupWeapon(to_pickup);
                            if (retry) {
                                writer_print("You pick up the " + to_pickup->name);
                                room.dropped_weapons.erase(room.dropped_weapons.begin() + index - 1);
                            }
                        }
                    }
                }
            }
        }
        else if (choice == "h" || choice == "H") {
            if (room.dropped_healings.empty()) {
                writer_print("There are no healings on the floor");
                continue;
            }

            // show current healing inventory, mechanism same as weapons
            cout << "\n  Your healing inventory:" << endl;
            player.showHealings();
            cout << "  Capacity: " << player.healingsInv.size() << "/" << player.healingCap << endl;

            cout << "\n  Which healing to pick up? (1-" << room.dropped_healings.size() << ", or 0 to cancel): " << flush;
            int index;
            cin >> index;

            if (index == 0) continue;
            if (index < 1 || index > (int)room.dropped_healings.size()) {
                writer_print("Invalid choice");
                continue;
            }

            Healing* to_pickup = room.dropped_healings[index - 1];
            bool success = player.pickupHealings(to_pickup);

            if (success) {
                writer_print("You pick up the " + to_pickup->name);
                room.dropped_healings.erase(room.dropped_healings.begin() + index - 1);
            }
            else {
                writer_print("Your healing inventory is full!");
                writer_print("Use or discard a healing first, then try again");

                // offer to discard, can pick up instantly after discard
                cout << "\n  Discard a healing? (y/n): " << flush;
                string discard_choice;
                cin >> discard_choice;

                if (discard_choice == "y" || discard_choice == "Y") {
                    player.showHealings();
                    cout << "  Choose healing to discard (1-" << player.healingsInv.size()
                         << ", or 0 to cancel): " << flush;
                    int discard_index;
                    cin >> discard_index;

                    if (discard_index > 0 && discard_index <= (int)player.healingsInv.size()) {
                        Healing* discarded = player.healingsInv[discard_index - 1];
                        bool discarded_success = player.discardHealings(discard_index - 1);

                        if (discarded_success) {
                            writer_print("You discard the " + discarded->name);
                            drop_healing_in_room(room, discarded);

                            // Try pickup again
                            bool retry = player.pickupHealings(to_pickup);
                            if (retry) {
                                writer_print("You pick up the " + to_pickup->name);
                                room.dropped_healings.erase(
                                    room.dropped_healings.begin() + index - 1);
                            }
                        }
                    }
                }
            }
        }
        else {
            writer_print("Invalid choice. Type w, h, or n");
        }
    }
}

