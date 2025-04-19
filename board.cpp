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
    for (int row = 1 ; row < BOARD_HEIGHT-1 ; row++)
        for (int col = 1 ; col < BOARD_WIDTH-1 ; col++){
                this->board[row][col] = ' ';
        }
};

Board :: Board() {
    for (int row = 0 ; row < BOARD_HEIGHT ; row++)
        for (int col = 0 ; col < BOARD_WIDTH ; col++){
            if (row == 0 || row == BOARD_HEIGHT-1 || col == 0 || col == BOARD_WIDTH-1)
                this->board[row][col] = '+';
            else
                this->board[row][col] = ' ';
        }
};

void Board::setBoardCell(Point p, char ch) {
    this -> board[p.getX()][p.getY()] = ch;
};


void Board :: display() const{
    for (int row = 0 ; row < BOARD_HEIGHT ; row++)
        for (int col = 0; col < BOARD_WIDTH ; col++){
            gotoxy(col, row);
            std::cout << this->board[row][col];
        }
};



