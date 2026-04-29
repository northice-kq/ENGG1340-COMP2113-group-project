#include "output_text.h" 

#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

//  Please read 'output_text.h' for description of the following functions
void writer_print(const string& text, bool is_dots, bool is_new_line, int char_delay, int dot_delay) {
    
    // Print each character one at a time, flushing after each
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(char_delay));
    }

    // Brief pause so the player absorbs the text before the trailing dots begin
    // It will be stylish
    this_thread::sleep_for(chrono::milliseconds(200));

    // Print three dots, each slower than the characters
    // Creating a "voice trailing off" feeling-
    if (is_dots) {
        bool skip_dots = false;
        // Get the last character of the string
        if (!text.empty()) {
            char last_char = text.back();
            if (last_char == '!' || last_char == '?' || last_char == ')' || last_char == ':') {
                this_thread::sleep_for(chrono::milliseconds(200));
                // Make sure we do not create dots if there are other punctuations
                skip_dots = true; // skip dots as we do not need tem when there are ! or ?
            }
        }
        if (!skip_dots) {
            for (int i = 0; i < 3; i++) {
                cout << '.' << flush;
                this_thread::sleep_for(chrono::milliseconds(dot_delay));
            }
        }
        this_thread::sleep_for(chrono::milliseconds(450)); // extra pause for sentence break
    }
    if (is_new_line) {
        cout << "\n";
    }
}

void press_enter_to_continue() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "[Press Enter to continue...]" << flush;
    cin.get();
}

void scene_break() {
    // Use writer_print with faster character speed for the divider
    // no dots
    writer_print("*************************",false, true, 10, 200);

    // Extra pause after the divider fades in
    this_thread::sleep_for(chrono::milliseconds(200));
}

