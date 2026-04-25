#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H

// What it does: Provides utility functions for styled text output.
//              Includes typewriter-style character-by-character printing
//              with trailing dots, scene breaks, and other
//              atmospheric text effects for the game

#include <string>
using namespace std;

//
// What it does: Prints text character by character with a typewriter effect.
//              After the text finishes, prints  three dots "...",
//              each dot appearing slower than the characters, creating
//              trailing-off dramatic effect. can be used for room descriptions,
//               dialogue, story moments, and atmospheric narration.
// Inputs:  text (const string&) - the message to display
//          char_delay (int) - milliseconds delay between each character
//                                 (default 40ms for normal pace)
//          dot_delay (int)  - milliseconds delay between each dot
//                                 (default 200ms for dramatic trailing)

//          NOTE!!!! To use the default settings, omit the last three perimeters

void writer_print(const string& text, bool is_dots = true, int char_delay = 40, int dot_delay = 200);

//
// What it does: waits for the player to press Enter before continuing.
//              let player acknowledge info before continuing

void press_enter_to_continue();

//
// What it does: Prints a short horizontal divider and pauses briefly.
//              Used to separate sections of the game
//              No input is needed

void scene_break();

#endif
