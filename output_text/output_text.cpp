#include "output_text.h" 

#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

//  Please read 'output_text.h' for description of the following functions


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

