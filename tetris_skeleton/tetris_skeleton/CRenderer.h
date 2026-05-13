#pragma once
// ╔══════════════════════════════════════════╗
//  CRenderer.h  –  Người làm: Linh
//  Toàn bộ logic vẽ lên terminal
// ╚══════════════════════════════════════════╝
#include "CBoard.h"
#include "CScoreManager.h"

class CRenderer {
public:
    // Di chuyển con trỏ terminal đến tọa độ (col, row)
    // TODO: dùng ANSI escape "\033[row+1;col+1H" (Linux) hoặc SetConsoleCursorPosition (Windows)
    static void gotoxy(int col, int row);

    // In mã màu ANSI tương ứng với ký tự block (I/O/T/S/Z/J/L)
    // TODO: switch-case theo c, cout << CYAN/YELLOW/...
    static void applyColor(char c);

    // Căn phải chuỗi số val trong width ký tự (dùng để in điểm)
    // Ví dụ: padLeft(42, 5) → "   42"
    // TODO: to_string(val), thêm khoảng trắng trước khi đủ width
    static std::string padLeft(int val, int width);

    // Vẽ toàn bộ frame:  bàn chơi | bảng điều khiển | preview block tiếp theo
    // TODO: gotoxy(0,0), rồi duyệt i=0..H-1, từng dòng vẽ 3 cột
    void draw(const CBoard& board,
              const CBlock& nextBlock,
              const CScoreManager& score,
              bool paused) const;

    // Vẽ màn hình GAME OVER với kỷ lục, điểm, số hàng
    // TODO: gotoxy(0, H+2), in khung ASCII art
    void drawGameOver(const CScoreManager& score) const;
};
