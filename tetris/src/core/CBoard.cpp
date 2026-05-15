// ╔══════════════════════════════════════════╗
//  CBoard.cpp  –  Người làm: Thuận
// ╚══════════════════════════════════════════╝
#include "CBoard.h"

CBoard::CBoard() { init(); }

void CBoard::init() {
    for (int i = 0; i < BOARD_H; i++) {
        for (int j = 0; j < BOARD_W; j++) {
            if ((i == BOARD_H - 1) || (j == 0) || (j == BOARD_W - 1)) {
                grid[i][j] = '#';
            } else {
                grid[i][j] = ' ';
            }
        }
    }
}

bool CBoard::canMove(const CBlock& blk, int x, int y, int dx, int dy) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blk.shape[i][j] != ' ') {
                int tx = x + j + dx;
                int ty = y + i + dy;
                
                if (tx < 1 || tx >= BOARD_W - 1 || ty >= BOARD_H - 1) return false;
                if (grid[ty][tx] != ' ') return false;
            }
        }
    }
    return true;
}

void CBoard::removeBlock(const CBlock& blk, int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blk.shape[i][j] != ' ' && y + i < BOARD_H) {
                grid[y + i][x + j] = ' ';
            }
        }
    }
}

void CBoard::placeBlock(const CBlock& blk, int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blk.shape[i][j] != ' ') {
                grid[y + i][x + j] = blk.shape[i][j];
            }
        }
    }
}

int CBoard::clearLines(const std::function<void()>& drawCallback) {
    int linesCleared = 0;
    
    for (int i = BOARD_H - 2; i > 0; i--) {
        bool isFull = true;
        for (int j = 1; j < BOARD_W - 1; j++) {
            if (grid[i][j] == ' ') {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            for (int ii = i; ii > 0; ii--) {
                for (int jj = 1; jj < BOARD_W - 1; jj++) {
                    grid[ii][jj] = grid[ii - 1][jj];
                }
            }
            
            linesCleared++;
            i++; 
            
            if (drawCallback) {
                drawCallback();
            }
        }
    }
    return linesCleared;
}