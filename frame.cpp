#include <iostream>
#include <ctime>
#include <cstdlib>
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
        {{' ',' ',' ',' '}, {' ','T',' ',' '}, {'T','T','T',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','S','S',' '}, {'S','S',' ',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {'Z','Z',' ',' '}, {' ','Z','Z',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {'J',' ',' ',' '}, {'J','J','J',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ',' ','L',' '}, {'L','L','L',' '}, {' ',' ',' ',' '}}
};

char currentBlock[4][4];
int x = 4, y = 0, b = 1;
bool isPaused = false;

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
    COORD coord; coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    cout << "\033[" << y + 1 << ";" << x + 1 << "H";
#endif
}

int get_key() {
#ifdef _WIN32
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) return _getch();
        return ch;
    }
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    if (ch == 27) {
        getchar(); ch = getchar();
        if (ch == 'A') ch = 72; 
        else if (ch == 'B') ch = 80; 
        else if (ch == 'C') ch = 77; 
        else if (ch == 'D') ch = 75; 
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return ch;
#endif
    return -1;
}

void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000); 
#endif
}

void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (currentBlock[i][j] != ' ' && y+i < H)
                board[y+i][x+j] = ' ';
}

void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (currentBlock[i][j] != ' ' )
                board[y+i][x+j] = currentBlock[i][j];
}

void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if ((i==H-1) || (j==0) || (j == W-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

void draw(){
    gotoxy(0,0);
    cout << WHITE << "  ";
    for (int j = 1; j < W - 1; j++) cout << "══";
    cout << "   " << endl;

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
        
        if (i == 0)      cout << "   ║   CÁCH ĐIỀU KHIỂN  ║";
        else if (i == 1) cout << "   ╠════════════════════╣";
        else if (i == 2) cout << "   ║ ← / → : Sang Trái/Phải║";
        else if (i == 3) cout << "   ║ ↑     : Xoay Gạch     ║";
        else if (i == 4) cout << "   ║ ↓     : Rơi Nhanh     ║";
        else if (i == 5) cout << "   ║ Space : Rơi Tức Thì   ║";
        else if (i == 6) cout << "   ║ P     : Tạm Dừng      ║";
        else if (i == 7) cout << "   ╚════════════════════╝";
        if (i == 9 && isPaused) cout << "       [ ĐANG TẠM DỪNG ]";
        
        cout << endl;
    }
}

bool canMove(int dx, int dy, char tempBlock[4][4]){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (tempBlock[i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx < 1 || tx >= W - 1 || ty >= H - 1) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}

void rotateBlock() {
    char rotated[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            rotated[i][j] = currentBlock[3 - j][i];

    if (canMove(0, 0, rotated)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                currentBlock[i][j] = rotated[i][j];
    }
}

void removeLine(){
    for (int i = H-2; i > 0 ; i-- ){
        int count = 0;
        for (int j = 1; j < W-1 ; j++)
            if (board[i][j] != ' ') count++;
        if (count == W-2){
            for (int ii = i; ii > 0 ; ii-- )
                for (int jj = 1; jj < W-1 ; jj++ ) board[ii][jj] = board[ii-1][jj];
            i++;
        }
    }
}

void spawnBlock() {
    x = W/2 - 2; y = 0;
    b = rand() % 7; 
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            currentBlock[i][j] = blocks[b][i][j];
}

int main() {
    srand(time(0));
    initBoard();
    spawnBlock();
    int moveTimer = 0
    while (1) {
        int key = get_key();
        if (key == 'p' || key == 'P') isPaused = !isPaused;
        if (key == 'q' || key == 'Q') break;
        if (!isPaused) {
            boardDelBlock();
            if (key == 75 && canMove(-1, 0, currentBlock)) x--;
            if (key == 77 && canMove(1, 0, currentBlock)) x++;
            if (key == 80 && canMove(0, 1, currentBlock)) y++;
            if (key == 72) rotateBlock();
            if (key == 32) { 
                while (canMove(0, 1, currentBlock)) y++;
                moveTimer = 10; 
            }
            moveTimer++;
            if (moveTimer >= 10) {
                if (canMove(0, 1, currentBlock)) y++;
                else {
                    block2Board();
                    removeLine();
                    spawnBlock();
                    if (!canMove(0, 0, currentBlock)) {
                        draw();
                        gotoxy(W, H/2); cout << RED << "GAME OVER!" << RESET;
                        break;
                    }
                }
                moveTimer = 0;
            }
            block2Board();
        }
        draw();
        sleep_ms(30);
    }
    return 0;
}