#pragma once
#include "constants/constants.h"

class Point {
private:
    int x;
    int y;

public:
    Point(int x, int y);
    Point();
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    void move(Direction dir);
    bool operator==(Point other) const;
    Point operator+(Point other) const;
    Point NextPointIs(Direction dir) const;
};
