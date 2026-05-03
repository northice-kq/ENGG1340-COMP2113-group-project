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
./main
