#pragma once
// ╔══════════════════════════════════════════╗
//  CRenderer.h  –  Người làm: Linh
//  Toàn bộ logic vẽ lên terminal
// ╚══════════════════════════════════════════╝
#include "../core/CBoard.h"
#include "../core/CScoreManager.h"
#include <string>

class CRenderer {
public:
    // Di chuyển con trỏ terminal đến tọa độ (col, row)
    static void gotoxy(int col, int row);

    // In mã màu ANSI tương ứng với ký tự block (I/O/T/S/Z/J/L)
    static void applyColor(char c);

    // Căn phải chuỗi số val trong width ký tự (dùng để in điểm)
    static std::string padLeft(int val, int width);

    // Vẽ toàn bộ frame:  bàn chơi | bảng điều khiển | preview block tiếp theo
    void draw(const CBoard& board,
              const CBlock& nextBlock,
              const CScoreManager& score,
              bool paused) const;

    // Vẽ màn hình GAME OVER với kỷ lục, điểm, số hàng
    void drawGameOver(const CScoreManager& score) const;
};
