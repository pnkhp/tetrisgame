#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctime>
#include <cstdlib>

using namespace std;
#define H 20
#define W 15
char board[H][W] = {} ;
char blocks[][4][4] = {
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'I','I','I','I'},
         {' ',' ',' ',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','T',' ',' '},
         {'T','T','T',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','S','S',' '},
         {'S','S',' ',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'Z','Z',' ',' '},
         {' ','Z','Z',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'J',' ',' ',' '},
         {'J','J','J',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ',' ','L',' '},
         {'L','L','L',' '},
         {' ',' ',' ',' '}}
};

int x=4,y=0,b=1;

// --- CÁC HÀM HỖ TRỢ CHUẨN LINUX ---
void gotoxy(int x, int y) {
    cout << "\033[" << y + 1 << ";" << x + 1 << "H";
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

char getch(void) {
    char ch;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
// -----------------------------------

void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' && y+j < H)
                board[y+i][x+j] = ' ';
}

void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' )
                board[y+i][x+j] = blocks[b][i][j];
}

void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if ((i==H-1) || (j==0) || (j == W-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

// --- HÀM VẼ ĐÃ ĐƯỢC CẬP NHẬT ĐỂ THÊM KHUNG ---
void draw(){
    gotoxy(0,0);
    
    // Vẽ viền trên cùng (nắp đậy của game) và Viền trên của khung điều khiển
    cout << "╔═";
    for (int j = 1; j < W - 1; j++) cout << "══";
    cout << "═╗   ╔══════════════════╗" << endl;

    for (int i = 0 ; i < H ; i++){
        // Vẽ khu vực bảng Game
        for (int j = 0 ; j < W ; j++){
            if (board[i][j] == '#') {
                if (i == H - 1 && j == 0) cout << "╚═";
                else if (i == H - 1 && j == W - 1) cout << "═╝";
                else if (i == H - 1) cout << "══";
                else if (j == 0) cout << "║ ";
                else if (j == W - 1) cout << " ║";
            } else if (board[i][j] == ' ') {
                cout << "  "; 
            } else {
                cout << "[]"; 
            }
        }
        
        // Vẽ khung bảng điều khiển bên phải
        if (i == 0)      cout << "   ║  BẢNG ĐIỀU KHIỂN ║";
        else if (i == 1) cout << "   ╠══════════════════╣";
        else if (i == 2) cout << "   ║ [A] : Sang trái  ║";
        else if (i == 3) cout << "   ║ [D] : Sang phải  ║";
        else if (i == 4) cout << "   ║ [X] : Rơi nhanh  ║";
        else if (i == 5) cout << "   ║ [Q] : Thoát game ║";
        else if (i == 6) cout << "   ╚══════════════════╝";
        
        cout << endl;
    }
}
// ----------------------------------------------

bool canMove(int dx, int dy){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx<1 || tx >= W-1 || ty >= H-1) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}

void removeLine(){
    int j;
    for (int i = H-2; i >0 ; i-- ){
        for (j = 0; j < W-1 ; j++)
            if (board[i][j] == ' ') break;
        if (j == W-1){
            for (int ii = i; ii >0 ; ii-- )
                for (int j = 0; j < W-1 ; j++ ) board[ii][j] = board[ii-1][j];
            i++;
            draw();
            usleep(200000); 
        }
    }
}

int main()
{
    srand(time(0));
    b = rand() % 7;
    system("clear"); 
    initBoard();
    while (1){
        boardDelBlock();
        if (kbhit()){
            char c = getch();
            if (c=='a' && canMove(-1,0)) x--;
            if (c=='d' && canMove(1,0) ) x++;
            if (c=='x' && canMove(0,1))  y++;
            if (c=='q') break;
        }
        if (canMove(0,1)) y++;
        else {
            block2Board();
            removeLine();
            x = 5; y = 0; b = rand() % 7;
        }
        block2Board();
        draw();
        usleep(200000); 
    }
    return 0;
}