// ╔══════════════════════════════════════════╗
//  CGame.cpp  –  Người làm: Hoàng Nhật
// ╚══════════════════════════════════════════╝
#include "CGame.h"

CGame::CGame()
    : x(4), y(0), paused(false),
      moveTimer(0), dropLimit(12), bagIndex(8)
{
    for (int i = 0; i < 8; i++) bag[i] = i;
}

void CGame::shuffleBag() {
    for (int i = 0; i < 8; i++) {
        bag[i] = i;
    }
    for (int i = 7; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
    bagIndex = 0;
}

void CGame::updateNextBlock() {
    if (bagIndex >= 8) {
        shuffleBag();
    }
    next.load(bag[bagIndex]);
}

void CGame::spawnBlock() {
    x = 4;
    y = 0;
    current.copyFrom(next);
    bagIndex++;
    updateNextBlock();
}

void CGame::tryRotate() {
    CBlock r = current.rotated();
    if (board.canMove(r, x, y, 0, 0)) {
        current.copyFrom(r);
    }
}

void CGame::hardDrop() {
    while (board.canMove(current, x, y, 0, 1)) {
        y++;
    }
}

void CGame::handleInput(char c) {
    if (c == 27) { // ESC sequence for arrow keys
        char arrow = CInput::getArrowKey();
        if (!paused) {
            if (arrow == 'L' && board.canMove(current, x, y, -1, 0)) {
                x--;
            } else if (arrow == 'R' && board.canMove(current, x, y, 1, 0)) {
                x++;
            } else if (arrow == 'D' && board.canMove(current, x, y, 0, 1)) {
                y++;
            } else if (arrow == 'U') {
                tryRotate();
            }
        }
    } else {
        if (c == 'p' || c == 'P') {
            paused = !paused;
        } else if (!paused) {
            if ((c == 'a' || c == 'A') && board.canMove(current, x, y, -1, 0)) {
                x--;
            } else if ((c == 'd' || c == 'D') && board.canMove(current, x, y, 1, 0)) {
                x++;
            } else if ((c == 'x' || c == 'X' || c == 's' || c == 'S') && board.canMove(current, x, y, 0, 1)) {
                y++;
            } else if (c == 'w' || c == 'W') {
                tryRotate();
            } else if (c == ' ') {
                hardDrop();
            }
        }
    }
}

void CGame::run() {
    srand(time(0));
    scoreManager.load("highscore.txt");
    board.init();
    updateNextBlock();
    spawnBlock();

    bool isRunning = true;
    while (isRunning) {
        board.removeBlock(current, x, y);

        if (CInput::hasInput()) {
            char c = CInput::getChar();
            if (c == 'q' || c == 'Q') {
                isRunning = false;
            } else {
                handleInput(c);
            }
        }

        if (!paused && isRunning) {
            moveTimer++;
            if (moveTimer >= dropLimit) {
                if (board.canMove(current, x, y, 0, 1)) {
                    y++;
                } else {
                    board.placeBlock(current, x, y);
                    auto cb = [&](){ renderer.draw(board, next, scoreManager, paused);
                                     CInput::sleepMs(100); };
                    int lines = board.clearLines(cb);
                    if (lines > 0) {
                        scoreManager.addLines(lines);
                        dropLimit -= lines;
                        if (dropLimit < 1) dropLimit = 1;
                    }
                    spawnBlock();
                    if (!board.canMove(current, x, y, 0, 0)) isRunning = false; // game over
                }
                moveTimer = 0;
            }
        }

        if (isRunning) {
            board.placeBlock(current, x, y);
            renderer.draw(board, next, scoreManager, paused);
            CInput::sleepMs(30);
        }
    }

    scoreManager.save("highscore.txt");
    renderer.drawGameOver(scoreManager);
}