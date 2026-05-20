#!/bin/bash
clear

g++ tetris/src/*.cpp tetris/src/core/*.cpp tetris/src/system/*.cpp -o game

if [ $? -eq 0 ]; then
    ./game
else
    echo "[ERROR] Bien dich that bai!"
fi