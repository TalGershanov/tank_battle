#include "board.h"
#include <windows.h>
#include <cstdlib>

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
                this->board[row][col] = BOARDER_SYMBOL;
            else
                this->board[row][col] = ' ';
        }
};

void Board::setBoardCell(Point p, char ch) {
    this -> board[p.getX()][p.getY()] = ch;
};


void Board :: display(){
    //clrscr();
    for (int row = 0 ; row < BOARD_HEIGHT; row++){
        gotoxy(BEGIN_BOARD_X,BEGIN_BOARD_Y+row);
        for (int col = 0; col < BOARD_WIDTH; col++){
            gotoxy(BEGIN_BOARD_X+col, BEGIN_BOARD_Y + row);
            std::cout << this->board[row][col];
        }
        std::cout << std::endl;
    }
};

char Board :: getBoardCell (const Point p){
    return board[p.getX()][p.getY()];
};

bool Board::isEmptyPos (Point p){
    return (this->board[p.getX()][p.getY()] == ' ');
};


