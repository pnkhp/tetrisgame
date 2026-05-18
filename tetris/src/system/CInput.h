#pragma once
// ╔══════════════════════════════════════════╗
//  CInput.h  –  Người làm: Hồng Phúc
//  Xử lý bàn phím và sleep, tách biệt Windows/Linux
// ╚══════════════════════════════════════════╝
#include "../common.h"

class CInput {
public:
    // Kiểm tra có phím nào đang chờ không (non-blocking)
    static bool hasInput();

    // Đọc 1 ký tự (blocking, không echo ra màn hình)
    static char getChar();

    // Đọc phím mũi tên sau khi đã đọc ESC ('\033')
    // Trả về: 'L' trái | 'R' phải | 'U' lên | 'D' xuống
    static char getArrowKey();

    // Dừng chương trình ms mili-giây
    static void sleepMs(int ms);
};