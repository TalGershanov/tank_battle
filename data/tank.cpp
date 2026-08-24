#include "data/tank.h"

Tank::Tank(Point position, Direction initialDirection, bool hasCannon)
    : position(position), direction(initialDirection),
      leftTrackStatus(TrackStatus::STAY), rightTrackStatus(TrackStatus::STAY),
      cooldown(MIN_COOLDOWN), isHit(false), hasCannonFlag(hasCannon) {
    cannonPos = position;
    cannonPos.move(direction);
}

Point Tank::getPosition() const {
    return this->position;
}

Point Tank::getCannonPosition() const {
    return cannonPos;
}

void Tank::setPosition(Point p) {
    this->position = p;
}

void Tank::setCannonPosition(Point p) {
    this->cannonPos = p;
}

Direction Tank::getDirection() const {
    return this->direction;
}

void Tank::setDirection(Direction dir) {
    this->direction = dir;
}

TrackStatus Tank::getLeftTrackStatus() const {
    return this->leftTrackStatus;
}

TrackStatus Tank::getRightTrackStatus() const {
    return this->rightTrackStatus;
}

void Tank::setLeftTrackStatus(TrackStatus status) {
    this->leftTrackStatus = status;
}

void Tank::setRightTrackStatus(TrackStatus status) {
    this->rightTrackStatus = status;
}

bool Tank::hasCannon() const {
    return hasCannonFlag;
}

void Tank::clearCannon() {
    hasCannonFlag = false;
    this->cannonPos = this->position;
}

bool Tank::canShoot() const {
    return (this->cooldown == MIN_COOLDOWN) && hasCannonFlag;
}

void Tank::resetCooldown() {
    cooldown = MAX_COOLDOWN;
}

void Tank::cooldownTick() {
    if (cooldown > 0) cooldown--;
}

void Tank::markAsHit() {
    isHit = true;
}

bool Tank::isTankHit() const {
    return isHit;
}
