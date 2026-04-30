# ENGG1340-COMP2113-group-project

# Dungeon Crawler Game (C++)

## 1. Team Members

* Lam Ka Ho (UID: 3036216863)
* Ng Ho Him (UID: 3036392851)
* Wong Chun Ming (UID: 3036586969)
* Wong Shing Chun (UID: 3036392203)
* Li Xin Yue (UID: 3036589923)

## 2. Application Description

* This is a text-based and grid-based dungeon adventure game written in C++.
* Player needs to explore the grid, encounter enemies, and interact with different types of rooms such as combat, chest, and key rooms.
* Gameplay consists of moving through the grid and triggering room-based events.
* After reaching the escape room with three collected keys, the player wins.

## 3. Features

### 3.1 Grid-based Dungeon System

The game is built around a grid-based dungeon system where the entire map is represented as a 2D grid of rooms.

* The dungeon is a square grid (5×5 in Easy mode, 7×7 in Hard mode).
* Each grid cell is a `Room` object stored in a 2D dynamic array,    `vector<vector<Room>>`.
* Every room has a type, including: `START`, `ENEMY`, `CHEST`, `KEY`, `EMPTY`, and `ESCAPE`.
* Room types are randomly assigned at the beginning of the game using a shuffle-based algorithm.
* The player starts at position (0, 0), and movement is controlled using "up/down/left/right" commands.
* The map updates dynamically after each movement, revealing visited rooms.

Supported Coding Requirements:

* (1) Random events: Room types and their positions are randomly generated using `shuffle()`.
* (2) Data structures: The dungeon is stored using `vector<vector<Room>>`, and each `Room` contains state information.
* (5) Multiple files: Implemented in `Map.cpp`, `Map.h`,`room.cpp`.

### 3.2 Room Interaction System

Each room in the dungeon triggers different events when the player enters it.

* Empty Room: triggers atmospheric text and optional rest events.
* Chest Room: provides random loot such as weapons, healing items, or stat boosts.
* Key Room: contains one of the three required keys to progress.
* Enemy Room: triggers combat encounters.
* Escape Room: becomes available only after collecting all three keys and leads to victory.

Room states are tracked to ensure events only occur once per room unless specified otherwise.

Supported Coding Requirements:

* (1) Random events: Chest rewards, key placement, and room events are randomly generated.
* (2) Data structures: Each room is stored as a `Room` struct containing type, coordinates, and state flags (e.g. `revealed`, `chest_looted`).
* (5) Multiple files: Room logic is modularized in `room.cpp` and `room.h`.

### 3.3 Combat System

The combat system is triggered when the player enters an enemy room.

* The combat between the player and the enemy is turn-based.
* Player can attack, defend, or use items.
* Enemy's HP and attack damage scale based on the player's progression in the dungeon. 
* The hard mode introduces additional mechanics such as dodge-based reaction timing and random combat events.
* Weapons and player stats directly influence combat outcomes, including critical hits and damage scaling.

Supported Coding Requirements:

* (1) Random events: Damage variance, critical hits, and combat effects are randomly generated.
* (2) Data structures: Player and enemy stats are stored using structured classes (`Player`, `Enemy`).
* (3) Dynamic memory management: Memory is allocated to create a new `Enemy` object when the combat begins. It is released when the combat ends.
* (5) Multiple files: Implemented across `combatRoom.cpp`, `dodge.cpp`, and related modules.

### 3.4 Inventory and Item System

The player can collect and manage various weapons and healing items.

* Weapons include Sword, Axe, Calculator Gun, and Railgun.
* Healing items include Bandage, First Aid Kit, and Med Kit.
* Each item has unique effects and limitations.
* Inventory has capacity limits, requiring players to manage resources strategically.
* Items can be picked up from chests or dropped into rooms.

Supported Coding Requirements:

* (1) Random events: Item drops from chests are randomly generated.
* (2) Data structures: Inventory is stored using `vector<Weapon*>` and `vector<Healing*>`.
* (3) Dynamic memory management: Items are dynamically allocated using pointers and `new`.
* (5) Multiple files: Implemented across `weapon.cpp`, `healing.cpp`, and related files.

### 3.5 Key Collection and Escape Mechanism

Progression is based on collecting three keys hidden in the dungeon.

* Keys are randomly distributed across key rooms.
* The escape room is only generated after all three keys are collected.
* The escape room location is randomly selected but avoids the player’s current and nearby positions.
* Entering the escape room after collecting all keys results in victory.

Supported Coding Requirements:

* (1) Random events: Key placement and escape room generation are randomized.
* (2) Data structures: Grid state is updated dynamically to reveal escape room.
* (5) Multiple files: Implemented in `room.cpp` and `Map.cpp`.

### 3.6 Difficulty System

The game supports multiple difficulty levels:

Easy Mode:
* Larger inventory capacity
* More favorable loot distribution
* Weaker enemies

Hard Mode:
* Stronger enemies with higher scaling stats
* Reduced inventory capacity
* Additional combat randomness and dodge mechanics

Supported Coding Requirements:

* (1) Random events: Difficulty affects randomness of combat and rewards.
* (2) Data structures: Player and enemy stats adjust based on difficulty setting.
* (5) Multiple files: Difficulty logic is integrated into map initialization.
* (6) Multiple difficulty levels

## 4. Non-standard C/C++ Libraries

The following non-standard header files are included. They are used for a few low-level interactions with the Linux operating system.
* `<termios.h>`
* `<unistd.h>`
* `<fcntl.h>`

Other functionalities are implemented using standard C++ libraries, including:

* `<iostream>`, `<fstream>` and `<iomanip>` for input/output operations
* `<vector>` for dynamic data storage
* `<string>` and `<sstream>` for text handling
* `<algorithm>` for shuffling and utilities
* `<random>` and `<cstdlib>` for random number generation
* `<thread>` and `<chrono>` for timed effects and delays

These standard libraries support core game features such as random dungeon generation, combat randomness, and game flow control.

## 5. Compilation and Execution

Simply execute the `game.sh` bash script to compile and run the game.

Notes:
* g++ and make must be installed to compile the game.
* The program is designed to run on the Linux operating system.
* If compilation fails, ensure all .cpp and .h files are included in the correct directory.
