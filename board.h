#ifndef BOARD_H
#define BOARD_H
#include "general.h"
#include "point.h"


class Board {
private:
    char board[BOARD_HEIGHT][BOARD_WIDTH];
public:
    Board();
    void clear();
    void display() const;
    void gotoxy(int x, int y);
    void setBoardCell(Point p, char ch);
};







#endif //BOARD_H
