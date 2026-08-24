#include "data/mine.h"

Mine::Mine(Point p) : visibility(true), destroyed(false), position(p) {}
Mine::Mine() : visibility(true), destroyed(false) {}

Point Mine::getPosition() const {
    return position;
}

bool Mine::isDestroyed() const {
    return destroyed;
}

void Mine::setDestroyed() {
    destroyed = true;
}

bool Mine::isVisible() const {
    return visibility;
}

void Mine::markAsVisible() {
    visibility = true;
}

void Mine::markAsInvisible() {
    visibility = false;
}
