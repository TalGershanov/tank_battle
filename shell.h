
#ifndef SHELL_H
#define SHELL_H
#include "point.h"
#include "tank.h"
#include "board.h"


class Shell {
public:

    Shell(Point pos, Direction dir) : position(pos) , direction(dir){}
    void move(Board& board);
    Point getPosition() const;
    bool isActive() const;
    void deactivate();

    bool isAt(Point other) ;

    Direction getDirection();

private:
    Point position;
    Direction direction;
    bool active = true;
};

#endif //SHELL_H
