
#include "shell.h"




void Shell::move(Board& board) {
    board.setBoardCell(this->position, ' ');
    gotoxy(this->position.getY()+BEGIN_BOARD_X, this->position.getX()+BEGIN_BOARD_Y);
    std::cout << ' ';

    if (!this->active) {
        return;
    }

    this->position.move(this->direction);

    if (!((this->position.getX() >= MIN_ROW) && (this->position.getX() <= MAX_ROW) && (this->position.getY() >= MIN_COL) && (this->position.getY() <= MAX_COL)) )
        {
        if (this->position.getX() > MAX_ROW)
                this->position.setX(MIN_ROW);
        if (this->position.getX() < MIN_ROW)
            this->position.setX(MAX_ROW);
        if (this->position.getY() > MAX_COL)
            this->position.setY(MIN_COL);
        if (this->position.getY() < MIN_COL)
            this->position.setY(MAX_COL);

        }

    board.setBoardCell(this->position, SHELL_SYMBOL);
    gotoxy(this->position.getY() + BEGIN_BOARD_X, this->position.getX() + BEGIN_BOARD_Y);
    std::cout << SHELL_SYMBOL;
}


Point Shell::getPosition() const
{
    return position;
}

Direction Shell::getDirection()
{
    return direction;
}

bool Shell::isActive() const {
    return active;
}

void Shell::deactivate() {
    active = false;
}

bool Shell::isAt(Point other)  {
    return active && (position == other);
}
