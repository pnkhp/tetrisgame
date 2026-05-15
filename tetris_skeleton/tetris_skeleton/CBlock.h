#pragma once
// ╔══════════════════════════════════════════╗
//  CBlock.h  –  Người làm: Thuận
//  Quản lý hình dạng và phép xoay của khối
// ╚══════════════════════════════════════════╝
#include "common.h"

class CBlock {
public:
    char shape[4][4];   // Ma trận hình dạng hiện tại
    int  type;          // Loại block (0-7)

    // ── Khởi tạo, shape toàn ' ' ──
    CBlock();

    // Nạp hình dạng theo loại (0-7) từ BLOCK_SHAPES
    // TODO: copy BLOCK_SHAPES[blockType] vào shape, lưu type
    void load(int blockType);

    // Trả về bản sao đã xoay 90° theo chiều kim đồng hồ
    // (KHÔNG sửa *this, để caller kiểm tra va chạm trước)
    // TODO: áp dụng công thức rotated[j][3-i] = shape[i][j]
    CBlock rotated() const;

    // Sao chép toàn bộ dữ liệu từ other
    // TODO: copy type và toàn bộ shape[][]
    void copyFrom(const CBlock& other);

private:
    // Đặt toàn bộ shape[][] về ' '
    void clear();
};
