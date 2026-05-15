#pragma once
// ╔══════════════════════════════════════════╗
//  CBoard.h  –  Người làm: Thuận
//  Quản lý lưới, va chạm, xóa hàng
// ╚══════════════════════════════════════════╝
#include "CBlock.h"

class CBoard {
public:
    char grid[BOARD_H][BOARD_W]; // Lưới bàn chơi

    CBoard();

    // Khởi tạo lưới: tường '#' xung quanh, bên trong toàn ' '
    // TODO: vẽ tường trái/phải (j==0, j==W-1), đáy (i==H-1)
    void init();

    // Kiểm tra khối blk tại (x,y) có thể dịch thêm (dx,dy) không
    // Trả về false nếu ra ngoài biên hoặc đụng ô đã có ký tự
    // TODO: duyệt shape[i][j] != ' ', tính tx=x+j+dx, ty=y+i+dy
    bool canMove(const CBlock& blk, int x, int y, int dx, int dy) const;

    // Xóa vết khối khỏi lưới (gọi trước khi vẽ lại vị trí mới)
    // TODO: với ô blk.shape[i][j] != ' ' và y+i < H, gán grid[y+i][x+j]=' '
    void removeBlock(const CBlock& blk, int x, int y);

    // Ghi khối vào lưới (khóa vị trí sau khi block chạm đáy)
    // TODO: với ô blk.shape[i][j] != ' ', gán grid[y+i][x+j] = blk.shape[i][j]
    void placeBlock(const CBlock& blk, int x, int y);

    // Xóa các hàng đầy từ dưới lên, gọi drawCallback() sau mỗi hàng
    // Trả về số hàng đã xóa
    // TODO: kiểm tra j=1..W-2 không có ' ', dịch các hàng xuống, i++
    int clearLines(const std::function<void()>& drawCallback);
};
