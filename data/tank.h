#pragma once
#include "constants/constants.h"
#include "data/point.h"

enum class TrackStatus {STAY, FORWARD, BACKWARD};

class Tank {
private:
    Point position;
    Point cannonPos;
    Direction direction;
    TrackStatus leftTrackStatus;
    TrackStatus rightTrackStatus;
    int cooldown;
    bool isHit;
    bool hasCannonFlag;

public:
    Tank(Point position, Direction initialDirection, bool hasCannon);
    Tank(const Tank&) = default;
    Tank& operator=(const Tank&) = default;
    Tank(Tank&&) = default;
    Tank& operator=(Tank&&) = default;

    Point getPosition() const;
    Point getCannonPosition() const;
    void setPosition(Point p);
    void setCannonPosition(Point p);

    Direction getDirection() const;
    void setDirection(Direction dir);

    TrackStatus getLeftTrackStatus() const;
    TrackStatus getRightTrackStatus() const;
    void setLeftTrackStatus(TrackStatus status);
    void setRightTrackStatus(TrackStatus status);

    bool hasCannon() const;
    void clearCannon(); // matches original removeCannon()'s data mutation: hasCannon=false, cannonPos=position

    bool canShoot() const;
    void resetCooldown();
    void cooldownTick();

    void markAsHit();
    bool isTankHit() const;
};
