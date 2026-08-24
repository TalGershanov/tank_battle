#pragma once
#include "constants/constants.h"
#include "data/point.h"

class Wall {
public:
    explicit Wall(Point position);

    Point getPosition() const;
    int getHitsNumber() const;
    void increaseHits();
    bool isDestroyed() const;

private:
    Point position;
    int hitsNumber;
};
