#include "dodge.h" 
#include "../output_text/output_text.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <termios.h> // for instant key responds
#include <unistd.h> //unix
#include <fcntl.h> // file control

void setNonBlocking(bool enabled) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    if (enabled) {
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_lflag &= ~ECHO; //hides the typed 'r'
        ttystate.c_cc[VMIN] = 1;
    } else {
        ttystate.c_lflag |= ICANON;
        ttystate.c_lflag |= ECHO; //show
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

// check for keypress
bool keyPressed() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}
// reads keypress
char getKey() {
    return getchar();
}

// instructions flash - asthetics :)
void flashInstruction(int flashCount, int flashDelayMs) {
    for (int i = 0; i < flashCount; i++) {
    
        std::cout << "\033[2A"; // cursor up 2 line
        std::cout << "\033[2K"; // clears
        
        // show/hide text: flashes
        if (i % 2 == 0) {
            std::cout << "Press 'r' when the 'o' is inside the brackets [   ]\n";
        } else {
            std::cout << "\n";
        }
        
        std::cout << "\033[1B"; //cursor down 1 line
        std::cout << std::flush;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(flashDelayMs));
    }
}

bool Dodge() {
    const int LINE_LENGTH = 40;
    const int BRACKET_START = 17;
    const int BRACKET_END = 21;
    const int FRAME_DELAY_MS = rand() % 40 + 40; // random dodging speed
    const int FLASH_DELAY_MS = 500;
    
    // Display initial header and line
    writer_print("\n=== DODGE THE ATTACK ===", false); // false mean no dots - @north_ice
    writer_print("Press 'r' when the 'o' is inside the brackets [   ]", false); // false mean no dots - @north_ice
    
    // Draw the initial line with 'o' at position 0
    for (int pos = 0; pos < LINE_LENGTH; pos++) {
        if (pos >= BRACKET_START && pos <= BRACKET_END) {
            if (pos == BRACKET_START) {
                std::cout << "[";
            } else if (pos == BRACKET_END) {
                std::cout << "]";
            } else {
                std::cout << " ";
            }
        }
        else if (pos == 0) {
            std::cout << "o";
        }
        else {
            std::cout << "-";
        }
    }
    std::cout << std::endl;
    
    std::cout << "\033[s";

    // Flash the instruction line 3 times
    flashInstruction(6, FLASH_DELAY_MS);
    
    // Ensure instruction is visible after flashing
    std::cout << "\033[2A\033[2K";
    std::cout << "Press 'r' when the 'o' is inside the brackets [   ]\n";
    std::cout << "\033[u" << std::flush;
    std::cout << "\033[1A";
    // PAUSE
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    setNonBlocking(true);
    
    bool dodged = false;
    int pressPosition = -1;
    
    // Animate - move 'o' from left to right on the line
    for (int oPosition = 0; oPosition < LINE_LENGTH; oPosition++) {
        std::cout << "\r"; 
        
        // the line
        for (int pos = 0; pos < LINE_LENGTH; pos++) {
            // brackets - fixed position
            if (pos >= BRACKET_START && pos <= BRACKET_END) {
                if (pos == BRACKET_START) {
                    std::cout << "[";
                } else if (pos == BRACKET_END) {
                    std::cout << "]";
                } else if (pos == oPosition && oPosition > BRACKET_START && oPosition < BRACKET_END) {
                    std::cout << "o";
                } else {
                    std::cout << " ";
                }
            }
            else if (pos == oPosition) {
                std::cout << "o";
            }
            else {
                std::cout << "-";
            }
        }
        
        std::cout << std::flush;
        
        // Check if player pressed 'r' 
        if (keyPressed()) {
            char key = getKey();
            if (key == 'r' || key == 'R') {
                pressPosition = oPosition;
                break;
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(FRAME_DELAY_MS));
    }
    
    std::cout << std::endl;
    
    setNonBlocking(false);
    
    // Check if dodge was successful
    if (pressPosition != -1) {
        if (pressPosition >= BRACKET_START && pressPosition <= BRACKET_END) {
            writer_print("\n [SUCCESS!] You dodged the attack!"); // allow slower output to let player has enough time to read the text - @north_ice
            dodged = true;
        } else {
            writer_print("\n [FAIL!] You could not dodge the attack!"); // same as above - @north_ice
            dodged = false;
        }
    } else {
        writer_print("\n [TIME'S UP!] You forgot to dodge!"); // same as above - @north_ice
        dodged = false;
    }
    
    return dodged;
}

