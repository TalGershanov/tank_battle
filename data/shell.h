#pragma once
#include "constants/constants.h"
#include "data/point.h"

class Shell {
public:
    Shell(Point pos, Direction dir) : position(pos), direction(dir) {}

    Point getPosition() const;
    void setPosition(Point p);
    Direction getDirection() const;
    bool isActive() const;
    void deactivate();
    bool isAt(Point other) const;

private:
    Point position;
    Direction direction;
    bool active = true;
};
