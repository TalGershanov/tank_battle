#pragma once
#include "data/point.h"

class Mine {
public:
    explicit Mine(Point p);
    Mine();

    Point getPosition() const;
    bool isDestroyed() const;
    void setDestroyed();
    bool isVisible() const;
    void markAsVisible();
    void markAsInvisible();

private:
    bool visibility;
    bool destroyed;
    Point position;
};
