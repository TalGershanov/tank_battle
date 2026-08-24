#include "logic/tank_movement.h"
#include "logic/char_utils.h"

char TankMovementLogic::cannonSymbolFor(Direction dir) {
    char cannon;
    switch (dir) {
        case Direction::UP: cannon = CANNON_UP_OR_DOWN_SYMBOL; break;
        case Direction::UP_LEFT: cannon = CANNON_UL_OR_DR_SYMBOL; break;
        case Direction::LEFT: cannon = CANNON_L_OR_R_SYMBOL; break;
        case Direction::DOWN_LEFT: cannon = CANNON_DL_OR_UR_SYMBOL; break;
        case Direction::DOWN: cannon = CANNON_UP_OR_DOWN_SYMBOL; break;
        case Direction::DOWN_RIGHT: cannon = CANNON_UL_OR_DR_SYMBOL; break;
        case Direction::RIGHT: cannon = CANNON_L_OR_R_SYMBOL; break;
        case Direction::UP_RIGHT: cannon = CANNON_DL_OR_UR_SYMBOL; break;
        default: cannon = EMPTY_CELL_SYMBOL; break;
    }
    return cannon;
}

void TankMovementLogic::advance(Tank& tank, Board& board, char tankSymbol) {
    if (tank.isTankHit()) return;

    Point position = tank.getPosition();
    Point cannonPos = tank.getCannonPosition();
    Direction direction = tank.getDirection();
    TrackStatus leftTrackStatus = tank.getLeftTrackStatus();
    TrackStatus rightTrackStatus = tank.getRightTrackStatus();
    bool hasCannon = tank.hasCannon();

    board.setBoardCell(position, EMPTY_CELL_SYMBOL);
    board.setBoardCell(cannonPos, EMPTY_CELL_SYMBOL);

    Point prevPosition = position;
    Point prevCannonPosition = cannonPos;
    Direction prevDirection = direction;

    if (leftTrackStatus == TrackStatus::FORWARD && rightTrackStatus == TrackStatus::FORWARD) {
        position.move(direction);
        cannonPos.move(direction);
    }

    if (leftTrackStatus == TrackStatus::BACKWARD && rightTrackStatus == TrackStatus::BACKWARD) {
        Direction movementDirection;
        switch (direction) {
            case Direction::UP: movementDirection = Direction::DOWN; break;
            case Direction::UP_RIGHT: movementDirection = Direction::DOWN_LEFT; break;
            case Direction::RIGHT: movementDirection = Direction::LEFT; break;
            case Direction::DOWN_RIGHT: movementDirection = Direction::UP_LEFT; break;
            case Direction::DOWN: movementDirection = Direction::UP; break;
            case Direction::DOWN_LEFT: movementDirection = Direction::UP_RIGHT; break;
            case Direction::LEFT: movementDirection = Direction::RIGHT; break;
            case Direction::UP_LEFT: movementDirection = Direction::DOWN_RIGHT; break;
            default: movementDirection = Direction::NONE; break;
        }
        cannonPos = position;
        position.move(movementDirection);
    }

    if ((leftTrackStatus == TrackStatus::FORWARD && rightTrackStatus == TrackStatus::STAY) ||
        (leftTrackStatus == TrackStatus::STAY && rightTrackStatus == TrackStatus::BACKWARD)) {
        switch (direction) {
            case Direction::UP: direction = Direction::UP_RIGHT; break;
            case Direction::UP_RIGHT: direction = Direction::RIGHT; break;
            case Direction::RIGHT: direction = Direction::DOWN_RIGHT; break;
            case Direction::DOWN_RIGHT: direction = Direction::DOWN; break;
            case Direction::DOWN: direction = Direction::DOWN_LEFT; break;
            case Direction::DOWN_LEFT: direction = Direction::LEFT; break;
            case Direction::LEFT: direction = Direction::UP_LEFT; break;
            case Direction::UP_LEFT: direction = Direction::UP; break;
            default: break;
        }
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
        cannonPos = position;
        cannonPos.move(direction);
    }

    if ((leftTrackStatus == TrackStatus::BACKWARD && rightTrackStatus == TrackStatus::STAY) ||
        (leftTrackStatus == TrackStatus::STAY && rightTrackStatus == TrackStatus::FORWARD)) {
        switch (direction) {
            case Direction::UP: direction = Direction::UP_LEFT; break;
            case Direction::UP_LEFT: direction = Direction::LEFT; break;
            case Direction::LEFT: direction = Direction::DOWN_LEFT; break;
            case Direction::DOWN_LEFT: direction = Direction::DOWN; break;
            case Direction::DOWN: direction = Direction::DOWN_RIGHT; break;
            case Direction::DOWN_RIGHT: direction = Direction::RIGHT; break;
            case Direction::RIGHT: direction = Direction::UP_RIGHT; break;
            case Direction::UP_RIGHT: direction = Direction::UP; break;
            default: break;
        }
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
        cannonPos = position;
        cannonPos.move(direction);
    }

    if (leftTrackStatus == TrackStatus::FORWARD && rightTrackStatus == TrackStatus::BACKWARD) {
        switch (direction) {
            case Direction::UP: direction = Direction::RIGHT; break;
            case Direction::UP_RIGHT: direction = Direction::DOWN_RIGHT; break;
            case Direction::RIGHT: direction = Direction::DOWN; break;
            case Direction::DOWN_RIGHT: direction = Direction::DOWN_LEFT; break;
            case Direction::DOWN: direction = Direction::LEFT; break;
            case Direction::DOWN_LEFT: direction = Direction::UP_LEFT; break;
            case Direction::LEFT: direction = Direction::UP; break;
            case Direction::UP_LEFT: direction = Direction::UP_RIGHT; break;
            default: break;
        }
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
        cannonPos = position;
        cannonPos.move(direction);
    }

    if (leftTrackStatus == TrackStatus::BACKWARD && rightTrackStatus == TrackStatus::FORWARD) {
        switch (direction) {
            case Direction::UP: direction = Direction::LEFT; break;
            case Direction::UP_LEFT: direction = Direction::DOWN_LEFT; break;
            case Direction::LEFT: direction = Direction::DOWN; break;
            case Direction::DOWN_LEFT: direction = Direction::DOWN_RIGHT; break;
            case Direction::DOWN: direction = Direction::RIGHT; break;
            case Direction::DOWN_RIGHT: direction = Direction::UP_RIGHT; break;
            case Direction::RIGHT: direction = Direction::UP; break;
            case Direction::UP_RIGHT: direction = Direction::UP_LEFT; break;
            default: break;
        }
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
        cannonPos = position;
        cannonPos.move(direction);
    }

    if (!hasCannon) {
        cannonPos = position;
    }

    if (leftTrackStatus == TrackStatus::FORWARD && rightTrackStatus == TrackStatus::FORWARD) {
        if (hasCannon) {
            if (cannonPos.getY() > MAX_COL) position.setY(MIN_COL);
            if (cannonPos.getY() < MIN_COL) position.setY(MAX_COL);
            if (cannonPos.getX() < MIN_ROW) position.setX(MAX_ROW);
            if (cannonPos.getX() > MAX_ROW) position.setX(MIN_ROW);

            cannonPos = position;
            cannonPos.move(direction);
        } else {
            if (position.getY() > MAX_COL) position.setY(MIN_COL);
            if (position.getY() < MIN_COL) position.setY(MAX_COL);
            if (position.getX() < MIN_ROW) position.setX(MAX_ROW);
            if (position.getX() > MAX_ROW) position.setX(MIN_ROW);

            cannonPos = position;
        }
    }

    if (leftTrackStatus == TrackStatus::BACKWARD && rightTrackStatus == TrackStatus::BACKWARD) {
        if (hasCannon) {
            if (position.getY() > MAX_COL) position.setY(MIN_COL + 1);
            if (position.getY() < MIN_COL) position.setY(MAX_COL - 1);
            if (position.getX() < MIN_ROW) position.setX(MAX_ROW - 1);
            if (position.getX() > MAX_ROW) position.setX(MIN_ROW + 1);

            cannonPos = position;
            cannonPos.move(direction);
        } else {
            if (position.getY() > MAX_COL) position.setY(MIN_COL);
            if (position.getY() < MIN_COL) position.setY(MAX_COL);
            if (position.getX() < MIN_ROW) position.setX(MAX_ROW);
            if (position.getX() > MAX_ROW) position.setX(MIN_ROW);

            cannonPos = position;
        }
    }

    char cannonPosOnBoard = board.getBoardCell(cannonPos);
    char tankPosOnBoard = board.getBoardCell(position);
    if (cannonPosOnBoard == STRONG_WALL_SYMBOL || cannonPosOnBoard == WEAK_WALL_SYMBOL || isLetter(cannonPosOnBoard) ||
        tankPosOnBoard == STRONG_WALL_SYMBOL || tankPosOnBoard == WEAK_WALL_SYMBOL || isLetter(tankPosOnBoard)) {
        position = prevPosition;
        cannonPos = prevCannonPosition;
        direction = prevDirection;
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
    }

    board.setBoardCell(position, tankSymbol);

    char cannon = cannonSymbolFor(direction);
    if (hasCannon) {
        board.setBoardCell(cannonPos, cannon);
    }

    tank.setPosition(position);
    tank.setCannonPosition(cannonPos);
    tank.setDirection(direction);
    tank.setLeftTrackStatus(leftTrackStatus);
    tank.setRightTrackStatus(rightTrackStatus);
}

void TankMovementLogic::removeDeadTankFromBoard(const Tank& tank, Board& board) {
    board.setBoardCell(tank.getPosition(), EMPTY_CELL_SYMBOL);
    board.setBoardCell(tank.getCannonPosition(), EMPTY_CELL_SYMBOL);
}

void TankMovementLogic::clearTankCannonFromBoard(Tank& tank, Board& board) {
    tank.clearCannon();
    board.setBoardCell(tank.getCannonPosition(), EMPTY_CELL_SYMBOL);
}
