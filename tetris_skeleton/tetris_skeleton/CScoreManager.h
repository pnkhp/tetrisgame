#pragma once
// ╔══════════════════════════════════════════╗
//  CScoreManager.h  –  Người làm: Long Nhật
//  Quản lý điểm số, kỷ lục, đọc/ghi file
// ╚══════════════════════════════════════════╝
#include "common.h"

class CScoreManager {
public:
    int score;       // Điểm hiện tại
    int highScore;   // Kỷ lục
    int totalLines;  // Tổng số hàng đã xóa

    CScoreManager();

    // Đọc kỷ lục từ file (nếu không có thì highScore giữ nguyên 0)
    // TODO: mở filename, đọc vào highScore
    void load(const std::string& filename);

    // Ghi kỷ lục ra file
    // TODO: mở filename, ghi highScore
    void save(const std::string& filename) const;

    // Tính điểm cho số hàng xóa được:
    //   1 hàng = 36đ, 2 hàng = 108đ, 3 hàng = 324đ, ...  (mỗi hàng thêm *3)
    // TODO: vòng lặp nhân pts *= 3 (lines-1) lần
    int calcScore(int lines) const;

    // Cộng điểm và cập nhật kỷ lục
    // TODO: score += calcScore(lines), totalLines += lines
    //       nếu score > highScore thì cập nhật highScore
    void addLines(int lines);
};
