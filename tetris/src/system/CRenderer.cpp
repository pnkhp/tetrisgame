// ╔══════════════════════════════════════════╗
//  CRenderer.cpp  –  Người làm: Linh
// ╚══════════════════════════════════════════╝
#include "CRenderer.h"
#include <iostream>

void CRenderer::gotoxy(int col, int row) {
#ifdef _WIN32
    COORD coord;
    coord.X = col;
    coord.Y = row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    std::cout << "\033[" << row + 1 << ";" << col + 1 << "H";
#endif
}

void CRenderer::applyColor(char c) {
    switch(c) {
        case 'I': std::cout << CYAN; break;
        case 'O': std::cout << YELLOW; break;
        case 'T': std::cout << MAGENTA; break;
        case 'S': std::cout << GREEN; break;
        case 'Z': std::cout << RED; break;
        case 'J': std::cout << BLUE; break;
        case 'L': std::cout << WHITE; break;
        default: std::cout << RESET;
    }
}

std::string CRenderer::padLeft(int val, int width) {
    std::string s = std::to_string(val);
    while ((int)s.size() < width) s = " " + s;
    return s;
}

void CRenderer::draw(const CBoard& board,
                     const CBlock& nextBlock,
                     const CScoreManager& score,
                     bool paused) const {
    gotoxy(0, 0);
    std::cout << WHITE << "╔═";
    for (int j = 1; j < BOARD_W - 1; j++) std::cout << "══";
    std::cout << "═╗   ╔══════════════════════╗   ╔══════════╗" << RESET << "\n";

    for (int i = 0 ; i < BOARD_H ; i++){
        for (int j = 0 ; j < BOARD_W ; j++){
            if (board.grid[i][j] == '#') {
                std::cout << WHITE;
                if (i == BOARD_H - 1 && j == 0) std::cout << "╚═";
                else if (i == BOARD_H - 1 && j == BOARD_W - 1) std::cout << "═╝";
                else if (i == BOARD_H - 1) std::cout << "══";
                else if (j == 0) std::cout << "║ ";
                else if (j == BOARD_W - 1) std::cout << " ║";
                std::cout << RESET;
            } else if (board.grid[i][j] == ' ') {
                std::cout << "  ";
            } else {
                applyColor(board.grid[i][j]);
                std::cout << "[]" << RESET;
            }
        }

        std::cout << "   "; 
        switch(i) {
            case 0:  std::cout << "║   BẢNG ĐIỀU KHIỂN    ║"; break;
            case 1:  std::cout << "╠══════════════════════╣"; break;
            case 2:  std::cout << "║ [A/←]   : Trái       ║"; break;
            case 3:  std::cout << "║ [D/→]   : Phải       ║"; break;
            case 4:  std::cout << "║ [W/↑]   : Xoay       ║"; break;
            case 5:  std::cout << "║ [X/↓]   : Rơi nhanh  ║"; break;
            case 6:  std::cout << "║ [Space] : Rơi ngay   ║"; break;
            case 7:  std::cout << "║ [P]     : Tạm dừng   ║"; break;
            case 8:  std::cout << "║ [Q]     : Thoát      ║"; break;
            case 9:  std::cout << "╠══════════════════════╣"; break;
            case 10: std::cout << "║      BẢNG ĐIỂM       ║"; break;
            case 11: std::cout << "╠══════════════════════╣"; break;
            case 12: std::cout << "║ K.Lục: " << RED    << padLeft(score.getHighScore(), 13) << RESET << " ║"; break;
            case 13: std::cout << "║ Điểm : " << YELLOW << padLeft(score.getScore(), 13)     << RESET << " ║"; break;
            case 14: std::cout << "║ Hàng : " << GREEN  << padLeft(score.getTotalLines(), 13) << RESET << " ║"; break;
            case 15: std::cout << "╠══════════════════════╣"; break;
            case 16: 
                if (paused) std::cout << "║ TT: " << RED << "   TẠM DỪNG    " << RESET << " ║";
                else        std::cout << "║ TT: Đang chơi        ║"; 
                break;
            case 17: std::cout << "║ Combo:               ║"; break;
            case 18: std::cout << "║ 1 hàng =  36đ        ║"; break;
            case 19: std::cout << "╚══════════════════════╝"; break;
            default: std::cout << "                        "; break;
        }

        std::cout << "   ";
        switch(i) {
            case 0:  std::cout << "║  BLOCK   ║"; break;
            case 1:  std::cout << "║          ║"; break;
            case 2: case 3: case 4: case 5: {
                int r = i - 2;
                std::cout << "║ ";
                for (int j = 0; j < 4; j++) {
                    if (nextBlock.shape[r][j] != ' ') {
                        applyColor(nextBlock.shape[r][j]);
                        std::cout << "[]";
                        std::cout << RESET;
                    } else {
                        std::cout << "  ";
                    }
                }
                std::cout << " ║";
                break;
            }
            case 6:  std::cout << "╚══════════╝"; break;
            default: std::cout << "            "; break;
        }
        std::cout << "\n";
    }
}

void CRenderer::drawGameOver(const CScoreManager& score) const {
    gotoxy(0, BOARD_H + 2);
    std::cout << WHITE << "╔══════════════════════════════╗\n";
    std::cout << "║        GAME OVER!            ║\n";
    std::cout << "║  Kỷ lục:       " << RED    << padLeft(score.getHighScore(), 8) << WHITE << "      ║\n";
    std::cout << "║  Điểm của bạn: " << YELLOW << padLeft(score.getScore(), 8)     << WHITE << "      ║\n";
    std::cout << "║  Số hàng xoá:  " << GREEN  << padLeft(score.getTotalLines(), 8)<< WHITE << "      ║\n";
    std::cout << "╚══════════════════════════════╝" << RESET << "\n";
}