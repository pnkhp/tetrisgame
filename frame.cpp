#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream> 

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

using namespace std;
#define H 20
#define W 15

#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"

char board[H][W] = {};
char blocks[][4][4] = {
        {{' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {'I','I','I','I'}, {' ',' ',' ',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','T',' ',' '}, {'T','T','T',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','S','S',' '}, {'S','S',' ',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {'Z','Z',' ',' '}, {' ','Z','Z',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {'J',' ',' ',' '}, {'J','J','J',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ',' ','L',' '}, {'L','L','L',' '}, {' ',' ',' ',' '}}
};
char currentBlock[4][4];

int x = 4, y = 0, b = 1;
int score = 0;
int highScore = 0; // Biến lưu kỷ lục
int totalLines = 0;
bool paused = false;

// Tính điểm
int calcScore(int lines) {
    if (lines <= 0) return 0;
    int pts = 36;
    for (int i = 1; i < lines; i++) pts *= 3;
    return pts;
}

// Đọc kỷ lục từ file
void loadHighScore() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

// Lưu kỷ lục vào file
void saveHighScore() {
    ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void applyColor(char c) {
    switch(c) {
        case 'I': cout << CYAN; break;
        case 'O': cout << YELLOW; break;
        case 'T': cout << MAGENTA; break;
        case 'S': cout << GREEN; break;
        case 'Z': cout << RED; break;
        case 'J': cout << BLUE; break;
        case 'L': cout << WHITE; break;
        default: cout << RESET;
    }
}

void gotoxy(int x, int y) {
#ifdef _WIN32
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << y + 1 << ";" << x + 1 << "H";
#endif
}

int check_kbhit(void) {
#ifdef _WIN32
    return _kbhit();
#else
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) { ungetc(ch, stdin); return 1; }
    return 0;
#endif
}

char get_input(void) {
#ifdef _WIN32
    return _getch();
#else
    char ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

// Xử lý phím mũi tên
char getArrowKey(void) {
#ifdef _WIN32
    char c = _getch();
    if (c == 0 || c == 224) {
        char c2 = _getch();
        if (c2 == 75) return 'L';  // Trái
        if (c2 == 77) return 'R';  // Phải
        if (c2 == 72) return 'U';  // Lên
        if (c2 == 80) return 'D';  // Xuống
    }
    return c;
#else
    char ch = getchar();
    if (ch == 27) {  // ESC
        char c2 = getchar();
        if (c2 == '[') {
            char c3 = getchar();
            if (c3 == 'A') return 'U';  // Lên
            if (c3 == 'B') return 'D';  // Xuống
            if (c3 == 'C') return 'R';  // Phải
            if (c3 == 'D') return 'L';  // Trái
        }
    }
    return ch;
#endif
}

void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

string padLeft(int val, int width) {
    string s = to_string(val);
    while ((int)s.size() < width) s = " " + s;
    return s;
}

void boardDelBlock() {
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (currentBlock[i][j] != ' ' && y+i < H)
                board[y+i][x+j] = ' ';
}

void block2Board() {
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (currentBlock[i][j] != ' ' )
                board[y+i][x+j] = currentBlock[i][j];
}

void initBoard() {
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if ((i==H-1) || (j==0) || (j == W-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

void draw() {
    gotoxy(0,0);
    cout << WHITE << "╔═";
    for (int j = 1; j < W - 1; j++) cout << "══";
    cout << "═╗   ╔══════════════════╗" << RESET << endl;

    for (int i = 0 ; i < H ; i++){
        for (int j = 0 ; j < W ; j++){
            if (board[i][j] == '#') {
                cout << WHITE;
                if (i == H - 1 && j == 0) cout << "╚═";
                else if (i == H - 1 && j == W - 1) cout << "═╝";
                else if (i == H - 1) cout << "══";
                else if (j == 0) cout << "║ ";
                else if (j == W - 1) cout << " ║";
                cout << RESET;
            } else if (board[i][j] == ' ') {
                cout << "  ";
            } else {
                applyColor(board[i][j]);
                cout << "[]" << RESET;
            }
        }

        // Panel bên phải 
        if      (i == 0)  cout << "   ║  BẢNG ĐIỀU KHIỂN ║";
        else if (i == 1)  cout << "   ╠══════════════════╣";
        else if (i == 2)  cout << "   ║ [←/→] : Di chuyển ║";
        else if (i == 3)  cout << "   ║ [↑]   : Xoay      ║";
        else if (i == 4)  cout << "   ║ [↓]   : Rơi nhanh ║";
        else if (i == 5)  cout << "   ║ [Space]: Rơi ngay ║";
        else if (i == 6)  cout << "   ║ [P]   : Tạm dừng  ║";
        else if (i == 7)  cout << "   ║ [Q]   : Thoát     ║";
        else if (i == 8)  cout << "   ╠══════════════════╣";
        else if (i == 9)  cout << "   ║    BẢNG ĐIỂM     ║";
        else if (i == 10) cout << "   ╠══════════════════╣";
        else if (i == 11) cout << "   ║ K.Lục:" << RED    << padLeft(highScore, 11)  << RESET << " ║";
        else if (i == 12) cout << "   ║ Điểm :" << YELLOW << padLeft(score, 11)      << RESET << " ║";
        else if (i == 13) cout << "   ║ Hàng :" << GREEN  << padLeft(totalLines, 11) << RESET << " ║";
        else if (i == 14) cout << "   ╠══════════════════╣";
        else if (i == 15) {
            cout << "   ║ Trạng thái: ";
            if (paused) cout << RED << "TẠM DỪNG" << RESET;
            else cout << "Đang chơi";
            cout << "  ║";
        }
        else if (i == 16) cout << "   ║ Combo:           ║";
        else if (i == 17) cout << "   ║  1 hàng =  36đ   ║";
        else if (i == 18) cout << "   ║  2 hàng = 108đ   ║";
        else if (i == 19) cout << "   ╚══════════════════╝";
        // Dòng cuối được thêm ở cuối vòng lặp

        cout << endl;
    }
}

bool canMove(int dx, int dy) {
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (currentBlock[i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx<1 || tx >= W-1 || ty >= H-1) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}

int removeLine() {
    int linesCleared = 0;
    for (int i = H-2; i > 0 ; i-- ) {
        int j;
        for (j = 1; j < W-1 ; j++)
            if (board[i][j] == ' ') break;
        if (j == W-1) {
            for (int ii = i; ii > 0 ; ii-- )
                for (int jj = 1; jj < W-1 ; jj++ ) board[ii][jj] = board[ii-1][jj];
            i++;
            draw();
            sleep_ms(100);
            linesCleared++;
        }
    }
    return linesCleared;
}

void rotateBlock() {
    char rotated[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            rotated[i][j] = ' ';

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            rotated[j][3 - i] = currentBlock[i][j];

    bool collision = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (rotated[i][j] != ' ') {
                int tx = x + j;
                int ty = y + i;
                if (tx < 1 || tx >= W - 1 || ty >= H - 1 || board[ty][tx] != ' ') {
                    collision = true;
                    break;
                }
            }
        }
        if (collision) break;
    }

    if (!collision)
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                currentBlock[i][j] = rotated[i][j];
}

// Rơi tức thì (Hard Drop)
void hardDrop() {
    while (canMove(0, 1)) {
        y++;
    }
}

void spawnBlock() {
    x = 4; 
    y = 0; 
    b = rand() % 8;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            currentBlock[i][j] = blocks[b][i][j];
        }
    }
}

int main() {
    srand(time(0));
    
    loadHighScore(); // Tải kỷ lục khi mở game
    
    initBoard();
    spawnBlock();

    int moveTimer = 0;
    int dropLimit = 12;

    while (1) {
        boardDelBlock();

        if (check_kbhit()) {
            char c = get_input();
            
            // Xử lý phím thông thường
            if (c == 27) {  // ESC (phím mũi tên)
                c = getArrowKey();
                if (c == 'L' && canMove(-1, 0)) x--;  // Mũi tên trái
                if (c == 'R' && canMove(1, 0)) x++;   // Mũi tên phải
                if (c == 'U') rotateBlock();           // Mũi tên lên
                if (c == 'D' && canMove(0, 1)) y++;   // Mũi tên xuống
            } else {
                if ((c == 'a' || c == 'A') && canMove(-1, 0)) x--;
                if ((c == 'd' || c == 'D') && canMove(1, 0)) x++;
                if ((c == 'x' || c == 'X') && canMove(0, 1)) y++;
                if ((c == 'w' || c == 'W')) rotateBlock();
                if (c == ' ') hardDrop();  // Space: Rơi tức thì
                if (c == 'p' || c == 'P') paused = !paused;  // P: Tạm dừng
                if (c == 'q' || c == 'Q') break;
            }
        }

        if (!paused) {
            moveTimer++;
            if (moveTimer >= dropLimit) {
                if (canMove(0, 1)) {
                    y++;
                } else {
                    block2Board();
                    int lines = removeLine();
                    if (lines > 0) {
                        score += calcScore(lines);
                        totalLines += lines;
                        
                        // Cập nhật kỷ lục ngay lập tức nếu vượt
                        if (score > highScore) {
                            highScore = score;
                        }

                        dropLimit -= lines;
                        if (dropLimit < 1) dropLimit = 1;
                    }
                    spawnBlock();
                    if (!canMove(0, 0)) break;
                }
                moveTimer = 0;
            }
        }

        block2Board();
        draw();
        sleep_ms(30);
    }
    
    saveHighScore(); // Lưu lại kỷ lục khi kết thúc game


    gotoxy(0, H + 2);
    cout << WHITE << "╔══════════════════════════════╗" << endl;
    cout << "║        GAME OVER!            ║" << endl;
    cout << "║  Kỷ lục:       " << RED    << padLeft(highScore, 8) << WHITE << "      ║" << endl;
    cout << "║  Điểm của bạn: " << YELLOW << padLeft(score, 8)     << WHITE << "      ║" << endl;
    cout << "║  Số hàng xoá:  " << GREEN  << padLeft(totalLines, 8)<< WHITE << "      ║" << endl;
    cout << "╚══════════════════════════════╝" << RESET << endl;

    return 0;
}