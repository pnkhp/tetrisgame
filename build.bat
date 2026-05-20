@echo off
chcp 65001 > nul
cls

g++ tetris/src/main.cpp tetris/src/CGame.cpp tetris/src/core/CBlock.cpp tetris/src/core/CBoard.cpp tetris/src/core/CScoreManager.cpp tetris/src/system/CInput.cpp tetris/src/system/CRenderer.cpp -o tetris.exe

if %errorlevel% equ 0 (
    tetris.exe
) else (
    echo [ERROR] Bien dich that bai!
    pause
)