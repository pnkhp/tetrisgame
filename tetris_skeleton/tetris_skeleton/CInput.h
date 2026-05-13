#pragma once
// ╔══════════════════════════════════════════╗
//  CInput.h  –  Người làm: Hồng Phúc
//  Xử lý bàn phím và sleep, tách biệt Windows/Linux
// ╚══════════════════════════════════════════╝
#include "common.h"

class CInput {
public:
    // Kiểm tra có phím nào đang chờ không (non-blocking)
    // TODO: Windows → _kbhit(); Linux → termios + O_NONBLOCK + getchar()
    static bool hasInput();

    // Đọc 1 ký tự (blocking, không echo ra màn hình)
    // TODO: Windows → _getch(); Linux → termios tắt ICANON+ECHO rồi getchar()
    static char getChar();

    // Đọc phím mũi tên sau khi đã đọc ESC ('\033')
    // Trả về: 'L' trái | 'R' phải | 'U' lên | 'D' xuống
    // TODO: Windows → _getch() đọc scancode (75/77/72/80)
    //       Linux  → getchar() đọc '[' rồi getchar() đọc A/B/C/D
    static char getArrowKey();

    // Dừng chương trình ms mili-giây
    // TODO: Windows → Sleep(ms); Linux → usleep(ms * 1000)
    static void sleepMs(int ms);
};
