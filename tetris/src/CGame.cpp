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
    // TODO
}

void CGame::updateNextBlock() {
    // TODO
}

void CGame::spawnBlock() {
    // TODO
}

void CGame::tryRotate() {
    // TODO
}

void CGame::hardDrop() {
    // TODO
}

void CGame::handleInput(char c) {
    // TODO
    // Gợi ý:
    //   if c == 27 → đọc arrow key (CInput::getArrowKey())
    //   else kiểm tra a/A/d/D/w/W/x/X/space/p/P/q/Q
}

void CGame::run() {
    // TODO
    // Gợi ý cấu trúc vòng lặp chính:
    //
    //   srand(time(0));
    //   scoreManager.load("highscore.txt");
    //   board.init();
    //   updateNextBlock();
    //   spawnBlock();
    //
    //   while (true) {
    //       board.removeBlock(current, x, y);
    //
    //       if (CInput::hasInput()) {
    //           char c = CInput::getChar();
    //           handleInput(c);
    //       }
    //
    //       if (!paused) {
    //           moveTimer++;
    //           if (moveTimer >= dropLimit) {
    //               if (board.canMove(current, x, y, 0, 1)) {
    //                   y++;
    //               } else {
    //                   board.placeBlock(current, x, y);
    //                   auto cb = [&](){ renderer.draw(board,next,scoreManager,paused);
    //                                    CInput::sleepMs(100); };
    //                   int lines = board.clearLines(cb);
    //                   if (lines > 0) {
    //                       scoreManager.addLines(lines);
    //                       dropLimit -= lines;
    //                       if (dropLimit < 1) dropLimit = 1;
    //                   }
    //                   spawnBlock();
    //                   if (!board.canMove(current, x, y, 0, 0)) break; // game over
    //               }
    //               moveTimer = 0;
    //           }
    //       }
    //
    //       board.placeBlock(current, x, y);
    //       renderer.draw(board, next, scoreManager, paused);
    //       CInput::sleepMs(30);
    //   }
    //
    //   scoreManager.save("highscore.txt");
    //   renderer.drawGameOver(scoreManager);
}
