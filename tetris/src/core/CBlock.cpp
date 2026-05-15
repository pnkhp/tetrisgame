// ╔══════════════════════════════════════════╗
//  CBlock.cpp  –  Người làm: Thuận
// ╚══════════════════════════════════════════╝
#include "CBlock.h"

const char BLOCK_SHAPES[8][4][4] = {
    {{' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}},
    {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {'I','I','I','I'}, {' ',' ',' ',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {' ','T',' ',' '}, {'T','T','T',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {' ','S','S',' '}, {'S','S',' ',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {'Z','Z',' ',' '}, {' ','Z','Z',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {'J',' ',' ',' '}, {'J','J','J',' '}, {' ',' ',' ',' '}},
    {{' ',' ',' ',' '}, {' ',' ','L',' '}, {'L','L','L',' '}, {' ',' ',' ',' '}}
};

void CBlock::clear() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            shape[i][j] = ' ';
        }
    }
}

CBlock::CBlock() : type(0) {
    clear();
}

void CBlock::load(int blockType) {
    type = blockType;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            shape[i][j] = BLOCK_SHAPES[type][i][j];
        }
    }
}

CBlock CBlock::rotated() const {
    CBlock b;
    b.type = type;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            b.shape[j][3 - i] = shape[i][j];
        }
    }
    return b;
}

void CBlock::copyFrom(const CBlock& other) {
    type = other.type;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            shape[i][j] = other.shape[i][j];
        }
    }
}