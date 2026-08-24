#include "data/shell.h"

Point Shell::getPosition() const {
    return position;
}

void Shell::setPosition(Point p) {
    position = p;
}

Direction Shell::getDirection() const {
    return direction;
}

bool Shell::isActive() const {
    return active;
}

void Shell::deactivate() {
    active = false;
}

bool Shell::isAt(Point other) const {
    return active && (position == other);
}
