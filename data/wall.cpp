#include "data/wall.h"

Wall::Wall(Point position) : position(position), hitsNumber(0) {}

Point Wall::getPosition() const {
    return this->position;
}

int Wall::getHitsNumber() const {
    return this->hitsNumber;
}

void Wall::increaseHits() {
    if (hitsNumber < MAX_WALL_HITS_NUM)
        this->hitsNumber++;
}

bool Wall::isDestroyed() const {
    return hitsNumber >= MAX_WALL_HITS_NUM;
}
