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

char board[H][W] = {} ;
char blocks[][4][4] = {
        {{' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}},
        {{' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}, {' ','I',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {'I','I','I','I'}, {' ',' ',' ',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','O','O',' '}, {' ','O','O',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','T',' ',' '}, {'T','T','T',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ','S','S',' '}, {'S','S',' ',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {'Z','Z',' ',' '}, {' ','Z','Z',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {'J',' ',' ',' '}, {'J','J','J',' '}, {' ',' ',' ',' '}},
        {{' ',' ',' ',' '}, {' ',' ','L',' '}, {'L','L','L',' '}, {' ',' ',' ',' '}}
};

int x=4,y=0,b=1;

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

void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000); 
#endif
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' && y+i < H)
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

void draw(){
    gotoxy(0,0);
    cout << WHITE << "╔═";
    for (int j = 1; j < W - 1; j++) cout << "══";
    cout << "═╗   ╔══════════════════╗" << endl;

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
int removeLine(){
    int j;
    int linesCleared = 0; 
    for (int i = H-2; i >0 ; i-- ){
        for (j = 1; j < W-1 ; j++)
            if (board[i][j] == ' ') break;
        if (j == W-1){
            for (int ii = i; ii >0 ; ii-- )
                for (int jj = 1; jj < W-1 ; jj++ ) board[ii][jj] = board[ii-1][jj];
            i++;
            draw();
            sleep_ms(100); 
            linesCleared++;
        }
    }
    return linesCleared; 
}

int main()
{
    srand(time(0));
    b = rand() % 16; 
    initBoard();
    
    int moveTimer = 0;
    int dropLimit = 12; 

    while (1){
        boardDelBlock();
        
        if (check_kbhit()){
            char c = get_input();
            if ((c=='a' || c=='A') && canMove(-1,0)) x--;
            if ((c=='d' || c=='D') && canMove(1,0) ) x++;
            if ((c=='x' || c=='X') && canMove(0,1))  y++;
            if (c=='q' || c=='Q') break;
        }

        moveTimer++;
        if (moveTimer >= dropLimit) {
            if (canMove(0,1)) y++;
            else {
                block2Board();
                
                
                int lines = removeLine(); 
                if (lines > 0) {
                    dropLimit -= lines; 
                    if (dropLimit < 1) dropLimit = 1; 
                }
                
                x = 4; y = 0; b = rand() % 16;
                if (!canMove(0,0)) break; 
            }
            moveTimer = 0;
        }

        block2Board();
        draw();
        sleep_ms(30);
    }
    return 0;
}