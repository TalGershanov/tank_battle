
#ifndef MINE_H
#define MINE_H
#include "point.h"


class Mine {
private:
    bool visibility;
    bool destroyed;
    Point position;
public:
    Mine(Point p);
    Mine();
    Point getPosition();
    void draw();
    void markAsVisible();
    void markAsInvisible();
    bool isDestroyed();
    void setDestroyed();

};



#endif //MINE_H
