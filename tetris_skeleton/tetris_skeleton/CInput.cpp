// ╔══════════════════════════════════════════╗
//  CInput.cpp  –  Người làm: Hồng Phúc
// ╚══════════════════════════════════════════╝
#include "CInput.h"

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

// ─────────────────────────────────────────────
//  hasInput()  –  Kiểm tra phím không chặn
// ─────────────────────────────────────────────
bool CInput::hasInput() {
#ifdef _WIN32
    return _kbhit() != 0;
#else
    // Lưu cài đặt terminal cũ
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Tắt chế độ đọc từng dòng (ICANON) và echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Chuyển stdin sang non-blocking
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    int ch = getchar();

    // Khôi phục terminal và chế độ blocking
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin); // Trả ký tự về buffer để getChar() đọc sau
        return true;
    }
    return false;
#endif
}

// ─────────────────────────────────────────────
//  getChar()  –  Đọc 1 ký tự, không echo
// ─────────────────────────────────────────────
char CInput::getChar() {
#ifdef _WIN32
    return static_cast<char>(_getch());
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Tắt ICANON (đọc từng ký tự, không cần Enter) và ECHO
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char ch = static_cast<char>(getchar());

    // Khôi phục terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

// ─────────────────────────────────────────────
//  getArrowKey()  –  Đọc scancode phím mũi tên
//  Gọi SAU KHI đã đọc được ESC ('\033')
// ─────────────────────────────────────────────
char CInput::getArrowKey() {
#ifdef _WIN32
    // Trên Windows, _getch() trả về 0 hoặc 0xE0 trước scancode
    char code = static_cast<char>(_getch());
    switch (static_cast<unsigned char>(code)) {
        case 75: return 'L'; // ←
        case 77: return 'R'; // →
        case 72: return 'U'; // ↑
        case 80: return 'D'; // ↓
        default: return code;
    }
#else
    // Trên Linux, chuỗi escape mũi tên là: ESC '[' A/B/C/D
    // Ký tự '[' chưa được đọc, đọc tiếp tại đây
    char bracket = static_cast<char>(getchar());
    if (bracket != '[') return bracket; // Không phải mũi tên

    char dir = static_cast<char>(getchar());
    switch (dir) {
        case 'A': return 'U'; // ↑
        case 'B': return 'D'; // ↓
        case 'C': return 'R'; // →
        case 'D': return 'L'; // ←
        default:  return dir;
    }
#endif
}

// ─────────────────────────────────────────────
//  sleepMs()  –  Tạm dừng ms mili-giây
// ─────────────────────────────────────────────
void CInput::sleepMs(int ms) {
#ifdef _WIN32
    Sleep(static_cast<DWORD>(ms));
#else
    usleep(static_cast<useconds_t>(ms) * 1000);
#endif
}