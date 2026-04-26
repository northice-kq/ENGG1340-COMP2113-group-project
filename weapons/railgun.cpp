// more complicated mechanics so put in a separate file
#include "weapon.h"
#include <chrono>
#include <climits>
#include <fcntl.h> // file control
#include <iostream>
#include <termios.h> // for instant key responds
#include <thread>
#include <unistd.h> //unix

Railgun::Railgun()
    : Weapon("Railgun", "Charge up your attack!", 20, INT_MAX, -1) {}

int Railgun::useWeapon() {
    int damage = baseDamage + baseDamage * (level - 1) / 5;
    // 00001111222233334444555544443333222211110000
    const int width = 4, maxBonus = 5;
    const int linewidth = width * (maxBonus * 2 + 1);
    // delay: 20 to 40 ms
    const int delay = rand() % 21 + 20;
    std::cout << "Press space bar when the power level reaches " << maxBonus
              << '!' << std::endl;
    { // setNonBlocking(true)
        termios ttystate;
        tcgetattr(STDIN_FILENO, &ttystate);
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_lflag &= ~ECHO;
        ttystate.c_cc[VMIN] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    }
    int bonus = 0;
    for (int i = 0;; i++) {
        int last = linewidth - 1;
        int pos = last - abs(i % (2 * last) - last);
        // print the line where the current position is replaced by ' '
        std::cout << "\r[ ";
        for (int j = 0; j < linewidth; j++) {
            if (pos == j)
                std::cout << ' ';
            else
                std::cout << (maxBonus - abs(j / width - maxBonus));
        }
        std::cout << " ]" << std::flush;

        bool keyPressed = false;
        {
            timeval tv = {0L, 0L};
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            keyPressed = (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0);
        }
        if (keyPressed) {
            char c = getchar();
            bonus = (maxBonus - abs(pos / width - maxBonus));
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
    { // setNonBlocking(false)
        termios ttystate;
        tcgetattr(STDIN_FILENO, &ttystate);
        ttystate.c_lflag |= ICANON;
        ttystate.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    }
    std::cout << "Your railgun has charged to power level " << bonus
              << std::endl;
    damage += bonus * 3;
    std::cout << "Dealt damage: " << damage << std::endl;
    return damage;
}
