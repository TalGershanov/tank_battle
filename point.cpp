#include "point.h"




Point::Point(int x , int y) { this->x = x; this->y = y;};
Point :: Point (){ x= 0; y= 0;};

int Point :: getY() const{
    return this->y;
};

int Point::getX() const {
    return this->x;
};

void Point :: setX(int x) {
    this->x = x;
};

void Point :: setY(int y) {
    this->y = y;
};

void Point::move(Direction dir) {
    switch (dir) {
        case Direction::UP:
            this->y+=1;
            break;
        case Direction::DOWN:
            this->y-=1;
            break;
        case Direction::RIGHT:
            this->x+=1;
            break;
        case Direction::LEFT:
            this->x-=1;
            break;
        case Direction::NONE:
            break;


    }

}
