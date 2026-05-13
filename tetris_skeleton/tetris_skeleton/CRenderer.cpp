// ╔══════════════════════════════════════════╗
//  CRenderer.cpp  –  Người làm: Linh
// ╚══════════════════════════════════════════╝
#include "CRenderer.h"

void CRenderer::gotoxy(int col, int row) {
    // TODO
}

void CRenderer::applyColor(char c) {
    // TODO
}

std::string CRenderer::padLeft(int val, int width) {
    // TODO
    return "";
}

void CRenderer::draw(const CBoard& board,
                     const CBlock& nextBlock,
                     const CScoreManager& score,
                     bool paused) const {
    // TODO
    // Gợi ý cấu trúc:
    //   gotoxy(0, 0);
    //   in dòng khung trên
    //   for (int i = 0; i < BOARD_H; i++) {
    //       vẽ cột trái  (board.grid)
    //       vẽ cột giữa  (bảng điều khiển + điểm)
    //       vẽ cột phải  (nextBlock preview)
    //       cout << "\n";
    //   }
}

void CRenderer::drawGameOver(const CScoreManager& score) const {
    // TODO
}
