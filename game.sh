#!/bin/bash

cd build
make >/dev/null 2>../error.txt
cd ..
if [ -s error.txt ]; then
    echo "Game compilation failed, see error.txt" >&2
    exit
else
    rm error.txt
fi
cd build
if [ -e gamedata.txt ]; then
    echo "Do you want to load the saved game data? (y/n)"
    read choice
    if [ $choice == "y" ] || [ $choice == "Y" ]; then
        ./main gamedata.txt
    else
        ./main
    fi
else
    ./main
fi
