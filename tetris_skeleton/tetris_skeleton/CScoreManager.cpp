// ╔══════════════════════════════════════════╗
//  CScoreManager.cpp  –  Người làm: Long Nhật
// ╚══════════════════════════════════════════╝
#include "CScoreManager.h"

CScoreManager::CScoreManager() : score(0), highScore(0), totalLines(0) {}

void CScoreManager::load(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void CScoreManager::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

int CScoreManager::calcScore(int lines) const {
    if (lines <= 0) return 0;
    int pts = 36;
    for (int i = 1; i < lines; i++) pts *= 3;
    return pts;
}

void CScoreManager::addLines(int lines) {
    if (lines > 0) {
        int gainedScore = calcScore(lines);
        score += gainedScore;
        totalLines += lines;
        if (score > highScore) {
            highScore = score;
        }
    }
}