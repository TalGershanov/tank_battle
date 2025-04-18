#ifndef TANK_H
#define TANK_H
#include "general.h"
#include "point.h"

class Board;

class Tank {
private:
    Point position;
    Direction direction;

public:
    Tank(Point p);
    Point getPosition() const;
    void draw(Board& board);
    void setDirection(Direction direction);
    void move();
};



#endif //TANK_H
