#include "logic/shell_movement.h"

void ShellMovementLogic::advance(Shell& shell, Board& board) {
    board.setBoardCell(shell.getPosition(), EMPTY_CELL_SYMBOL);

    if (!shell.isActive()) {
        return;
    }

    Point position = shell.getPosition();
    position.move(shell.getDirection());

    if (!((position.getX() >= MIN_ROW) && (position.getX() <= MAX_ROW) &&
          (position.getY() >= MIN_COL) && (position.getY() <= MAX_COL))) {
        if (position.getX() > MAX_ROW) position.setX(MIN_ROW);
        if (position.getX() < MIN_ROW) position.setX(MAX_ROW);
        if (position.getY() > MAX_COL) position.setY(MIN_COL);
        if (position.getY() < MIN_COL) position.setY(MAX_COL);
    }

    shell.setPosition(position);
    board.setBoardCell(position, SHELL_SYMBOL);
}
