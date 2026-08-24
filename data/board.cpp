#include "data/board.h"

Board::Board() {
    for (int row = 0; row < BOARD_HEIGHT; row++)
        for (int col = 0; col < BOARD_WIDTH; col++) {
            if (row == 0 || row == BOARD_HEIGHT - 1 || col == 0 || col == BOARD_WIDTH - 1)
                this->board[row][col] = BOARDER_SYMBOL;
            else
                this->board[row][col] = EMPTY_CELL_SYMBOL;
        }
}

void Board::clear() {
    for (int row = 1; row < BOARD_HEIGHT - 1; row++)
        for (int col = 1; col < BOARD_WIDTH - 1; col++) {
            this->board[row][col] = EMPTY_CELL_SYMBOL;
        }
}

void Board::setBoardCell(Point p, char ch) {
    this->board[p.getX()][p.getY()] = ch;
}

char Board::getBoardCell(Point p) const {
    return board[p.getX()][p.getY()];
}

bool Board::isEmptyPos(Point p) const {
    return this->board[p.getX()][p.getY()] == EMPTY_CELL_SYMBOL;
}
