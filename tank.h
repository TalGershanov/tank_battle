#ifndef TANK_H
#define TANK_H
#include "general.h"
#include "point.h"
enum class TrackStatus {STAY , FORWARD , BACKWARD};
class Board;

class Tank {
private:
    Point position;
    Point cannonPos;
    Direction direction;
    TrackStatus leftTrackStatus;
    TrackStatus rightTrackStatus;
    int cooldown;
    bool isHit;
    bool hasCannon;

public:
    Tank(Point p,Board &board);
    Tank(const Tank&) = delete;
    Tank& operator=(const Tank&) = delete;
    Tank(Tank&&) = default;
    Tank& operator=(Tank&&) = default;

    Point getPosition() const;
    Point getCannonPosition() const;
    void setDirection(Direction direction);
    void move(Board& board,char symbol);
    TrackStatus getRightTrackStatus();
    TrackStatus getLeftTrackStatus();
    void setRightTrackStatus(TrackStatus status);
    void setLeftTrackStatus(TrackStatus status);
    bool canShoot();
    void resetCooldown();
    void cooldownTick();
    Direction getDirection();
    void markAsHit();
    bool isTankHit();
    void removeCannon(Board& board);
    void removeDeadTank(Board& board);
};



#endif //TANK_H
