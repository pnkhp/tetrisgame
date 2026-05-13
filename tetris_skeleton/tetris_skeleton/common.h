#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// ── Kích thước bàn chơi ──
constexpr int BOARD_H = 20;
constexpr int BOARD_W = 15;

// ── Mã màu ANSI ──
#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"

// ── Dữ liệu hình dạng 8 loại block (chỉ đọc) ──
extern const char BLOCK_SHAPES[8][4][4];
