// ╔══════════════════════════════════════════╗
//  CBoard.cpp  –  Người làm: Thuận
// ╚══════════════════════════════════════════╝
#include "CBoard.h"

CBoard::CBoard() { init(); }

void CBoard::init() {
    // TODO
}

bool CBoard::canMove(const CBlock& blk, int x, int y, int dx, int dy) const {
    // TODO
    return false;
}

void CBoard::removeBlock(const CBlock& blk, int x, int y) {
    // TODO
}

void CBoard::placeBlock(const CBlock& blk, int x, int y) {
    // TODO
}

int CBoard::clearLines(const std::function<void()>& drawCallback) {
    // TODO
    return 0;
}
