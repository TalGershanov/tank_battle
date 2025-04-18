#include "tank.h"

#include "board.h"

Tank :: Tank(Point p) {
    this->position = p;
    this->direction = Direction::NONE;
};
Point Tank :: getPosition() const {
    return this->position;

};

void Tank::setDirection(Direction direction) {
    this->direction = direction;
}

void Tank :: draw(Board& board) {
    board.setBoardCell(this->position, 'O');
};

void Tank::move() {
    this->position.move(this->direction);
}
