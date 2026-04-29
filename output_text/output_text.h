#ifndef OUTPUT_TEXT_H
#define OUTPUT_TEXT_H



void writer_print(const string& text, bool is_dots = true, bool is_new_line = true, int char_delay = 20, int dot_delay = 100);

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
