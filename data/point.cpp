#include "data/point.h"

Point::Point(int x, int y) { this->x = x; this->y = y; }
Point::Point() { x = 0; y = 0; }

int Point::getY() const {
    return this->y;
}

int Point::getX() const {
    return this->x;
}

void Point::setX(int _x) {
    this->x = _x;
}

void Point::setY(int _y) {
    this->y = _y;
}

void Point::move(Direction dir) {
    switch (dir) {
        case Direction::UP:
            this->x -= 1;
            break;
        case Direction::UP_RIGHT:
            this->x -= 1;
            this->y += 1;
            break;
        case Direction::RIGHT:
            this->y += 1;
            break;
        case Direction::DOWN_RIGHT:
            this->x += 1;
            this->y += 1;
            break;
        case Direction::DOWN:
            this->x += 1;
            break;
        case Direction::DOWN_LEFT:
            this->x += 1;
            this->y -= 1;
            break;
        case Direction::LEFT:
            this->y -= 1;
            break;
        case Direction::UP_LEFT:
            this->x -= 1;
            this->y -= 1;
            break;
        case Direction::NONE:
            break;
    }
}

bool Point::operator==(Point other) const {
    return x == other.getX() && y == other.getY();
}

Point Point::operator+(Point other) const {
    return Point(x + other.getX(), y + other.getY());
}

Point Point::NextPointIs(Direction dir) const {
    Point tempP = *this;
    tempP.move(dir);
    return tempP;
}
