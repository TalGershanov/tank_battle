#ifndef POINT_H
#define POINT_H
#include "general.h"


class Point {
private:
    int x;
    int y;

public:
    Point(int x , int y);
    Point ();
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    void move(Direction dir);
    bool operator==( Point other);
    Point operator+( Point other);
    Point NextPointIs(Direction dir);
};


#endif //POINT_H
