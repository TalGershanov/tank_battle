#pragma once
#include "constants/constants.h"
#include "data/point.h"

class Board {
private:
    char board[BOARD_HEIGHT][BOARD_WIDTH];

public:
    Board();
    void clear();
    void setBoardCell(Point p, char ch);
    char getBoardCell(Point p) const;
    bool isEmptyPos(Point p) const;
};
