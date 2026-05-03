#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H

// What it does: Provides utility functions for styled text output.
//              Includes typewriter-style character-by-character printing
//              with trailing dots, scene breaks, and other
//              atmospheric text effects for the game

#include <string>
using namespace std;

// What it does: Prints text character by character with a typewriter effect.
//              After the text finishes, prints  three dots "...",
//              each dot appears slower than the characters, creating
//              trailing-off dramatic effect. can be used for room descriptions, dialogue, story moments, and atmospheric narration.
// Inputs:  text (const string&) - the message to display
//          char_delay - milliseconds delay between each character
//                                 (default 20ms for normal pace)
//          dot_delay    - milliseconds delay between each dot
//                                  (default 100ms for dramatic trailing and enhance readability)
// Outputs: same text, but with a typewriter effects and optional dotting

//          NOTE!!!! To use the default settings, omit the last two perimeters

void writer_print(const string& text, bool is_dots = true, bool is_new_line = true, int char_delay = 20, int dot_delay = 100);

//
// What it does: waits for the player to press Enter before continuing.
//              let player acknowledge info before continuing
//              accept an enter to continue the game

void press_enter_to_continue();

//
// What it does: Prints a short horizontal divider and pauses briefly.
//              Used to separate sections of the game
//              No input is needed
//              Outputs are multiple asterisks to seperate contents

void scene_break();

#endif
