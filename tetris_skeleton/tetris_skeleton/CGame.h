#pragma once
// ╔══════════════════════════════════════════╗
//  CGame.h  –  Người làm: Hoàng Nhật
//  Vòng lặp chính, túi 7-bag, kết nối tất cả
// ╚══════════════════════════════════════════╝
#include "CBoard.h"
#include "CScoreManager.h"
#include "CRenderer.h"
#include "CInput.h"

class CGame {
public:
    CGame();

    // Khởi tạo và chạy game đến khi kết thúc
    // TODO: srand, load highscore, init board, updateNextBlock, spawnBlock
    //       vòng while(true): xử lý input → logic rơi → vẽ → sleep
    void run();

private:
    CBoard        board;
    CBlock        current;     // Khối đang rơi
    CBlock        next;        // Khối tiếp theo (hiển thị preview)
    CScoreManager scoreManager;
    CRenderer     renderer;

    int  x, y;          // Vị trí khối hiện tại trên bàn
    bool paused;
    int  moveTimer;     // Đếm tick kể từ lần rơi cuối
    int  dropLimit;     // Số tick giữa 2 lần rơi (giảm khi level tăng)

    // ── Túi 7-bag ──
    int bag[8];
    int bagIndex;       // Con trỏ vào bag; khi >= 8 thì xáo lại

    // Xáo túi Fisher-Yates, reset bagIndex = 0
    // TODO: bag[i]=i, swap ngẫu nhiên, bagIndex=0
    void shuffleBag();

    // Cập nhật next từ bag[bagIndex] (xáo nếu cần)
    // TODO: if bagIndex >= 8 → shuffleBag(); next.load(bag[bagIndex])
    void updateNextBlock();

    // Đặt khối mới lên đầu bàn, lấy từ next, gọi updateNextBlock
    // TODO: x=4, y=0; current.copyFrom(next); bagIndex++; updateNextBlock()
    void spawnBlock();

    // Thử xoay current, chỉ áp dụng nếu không va chạm
    // TODO: block r = current.rotated(); if canMove(r,...) → current.copyFrom(r)
    void tryRotate();

    // Thả khối thẳng xuống đáy ngay lập tức
    // TODO: while canMove(0,1) → y++
    void hardDrop();

    // Xử lý một phím đã đọc được
    // TODO: switch/if theo ký tự: a/A/←, d/D/→, w/W/↑, x/X/↓, space, p/P, q/Q
    void handleInput(char c);
};
