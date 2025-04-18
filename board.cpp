#include "board.h"
#include <Windows.h>

void Board :: gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Board :: clear(){
    for (int i = 0 ; i < BOARD_HEIGHT ; i++)
        for (int j = 0 ; j < BOARD_WIDTH ; j++)
            this->board[i][j] = ' ';
};

Board :: Board() {
   this -> clear();
};

void Board::setBoardCell(Point p, char ch) {
    this -> board[p.getX()][p.getY()] = ch;
};


void Board :: display() const{
    for (int i = 0; i < BOARD_WIDTH+2; i++) {
            cout << '-';
    }

    cout << endl;
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        cout << "|";
        for (int j = 0; j < BOARD_WIDTH; j++) {
            cout << this->board[i][j];
        }
        cout << '|' <<endl;
    }

    for (int i = 0; i < BOARD_WIDTH+2; i++) {
        cout << '-';
    }
};



