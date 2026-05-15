// ╔══════════════════════════════════════════╗
//  main.cpp
// ╚══════════════════════════════════════════╝
#include "CGame.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    CGame game;
    game.run();
    return 0;
}
