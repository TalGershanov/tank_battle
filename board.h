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
    void display();
    void setBoardCell(Point p, char ch);
    char getBoardCell (const Point p);
    bool isEmptyPos (Point);

};







#endif //BOARD_H
