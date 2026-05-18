#pragma once
// ╔══════════════════════════════════════════╗
//  CScoreManager.h  –  Người làm: Long Nhật
//  Quản lý điểm số, kỷ lục, đọc/ghi file
// ╚══════════════════════════════════════════╝
#include "../common.h"

class CScoreManager {
private:
    int score;
    int highScore;
    int totalLines;

public:
    CScoreManager();

    void load(const std::string& filename);
    void save(const std::string& filename) const;
    int calcScore(int lines) const;
    void addLines(int lines);

    // Các hàm getter hỗ trợ cho CRenderer và CGame lấy dữ liệu (không làm thay đổi chữ ký hàm gốc)
    int getScore() const { return score; }
    int getHighScore() const { return highScore; }
    int getTotalLines() const { return totalLines; }
    void resetSession() { score = 0; totalLines = 0; }
};
