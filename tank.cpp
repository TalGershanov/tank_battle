#include "tank.h"

#include "board.h"

Tank :: Tank(Point p,Board &board) {
    position = p;
    //check if tank is surrounded and will be born with no cannon
    bool isSurrounded = true;
    for (int i = 0; i<8 ; i++) {
        if (board.isEmptyPos(p.NextPointIs(static_cast<Direction>(i))))
            isSurrounded = false;

    }
    if (!isSurrounded) {
        this->direction = static_cast<Direction>((rand()) % 8);
        while (!(board.isEmptyPos(p.NextPointIs(this->direction)))) {
            this->direction = static_cast<Direction>((rand()) % 8);
        }
        hasCannon = true;
    }
    else {
        hasCannon = false;
        this->direction = Direction::UP;
    }

    leftTrackStatus = TrackStatus::STAY;
    rightTrackStatus = TrackStatus::STAY;
    cannonPos = p;
    cannonPos.move(direction);
    cooldown = MIN_COOLDOWN;
    isHit = false;


};

Point Tank :: getPosition() const {
    return this->position;
};

void Tank::setDirection(Direction direction) {
    this -> direction = direction;
}

Point Tank :: getCannonPosition() const{
    return cannonPos;
};

Direction Tank :: getDirection() {
    return this->direction;
}

void Tank ::removeDeadTank(Board& board) {

    board.setBoardCell(this->position, ' ');
    gotoxy(this->position.getY() + BEGIN_BOARD_X, this->position.getX() + BEGIN_BOARD_Y);
    std::cout << ' ';
    board.setBoardCell(this->cannonPos, ' ');
    gotoxy(this->cannonPos.getY() + BEGIN_BOARD_X, this->cannonPos.getX() + BEGIN_BOARD_Y);
    std::cout << ' ';
};


void Tank::move(Board& board, char tankSymbol) {
    if (isHit) return;
    board.setBoardCell(this->position, ' ');
    gotoxy(this->position.getY()+BEGIN_BOARD_X, this->position.getX()+BEGIN_BOARD_Y);
    std::cout << ' ';
    board.setBoardCell(this->cannonPos, ' ');
    gotoxy(this->cannonPos.getY()+BEGIN_BOARD_X, this->cannonPos.getX()+BEGIN_BOARD_Y);
    std::cout << ' ';

    Point prevPosition = this->position;
    Point prevCannonPosition = this->cannonPos;
    Direction prevDirection = this->direction;

    if (leftTrackStatus == TrackStatus::FORWARD && rightTrackStatus == TrackStatus::FORWARD) {
        this->position.move(direction);
        this->cannonPos.move(direction);
    }

    if (leftTrackStatus == TrackStatus::BACKWARD && rightTrackStatus == TrackStatus::BACKWARD) {
        Direction movementDirection;
        switch (direction) {
            case Direction::UP: movementDirection = Direction::DOWN; break;
            case Direction::UP_RIGHT: movementDirection = Direction::DOWN_LEFT; break;
            case Direction::RIGHT: movementDirection = Direction::LEFT; break;
            case Direction::DOWN_RIGHT: movementDirection = Direction::UP_LEFT; break;
            case Direction::DOWN: movementDirection = Direction::UP_LEFT; break;
            case Direction::DOWN_LEFT: movementDirection = Direction::UP_RIGHT; break;
            case Direction::LEFT: movementDirection = Direction::RIGHT; break;
            case Direction::UP_LEFT: movementDirection = Direction::DOWN_RIGHT; break;
        }
        this->cannonPos = this->position;
        this->position.move(movementDirection);
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
        }
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
        this->cannonPos = this->position;
        this->cannonPos.move(direction);
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
        }
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
        this->cannonPos = this->position;
        this->cannonPos.move(direction);
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
        }
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
        this->cannonPos = this->position;
        this->cannonPos.move(direction);
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
        }
        leftTrackStatus = TrackStatus::STAY;
        rightTrackStatus = TrackStatus::STAY;
        this->cannonPos = this->position;
        this->cannonPos.move(direction);
    }

    if (!hasCannon) {
        this->cannonPos = this->position;
    }


    if (leftTrackStatus == TrackStatus::FORWARD && rightTrackStatus == TrackStatus::FORWARD) {
        if (hasCannon) {
            if (cannonPos.getY() > MAX_COL)
                position.setY(MIN_COL);
            if (cannonPos.getY() < MIN_COL)
                position.setY(MAX_COL);
            if (cannonPos.getX() < MIN_ROW)
                position.setX(MAX_ROW);
            if (cannonPos.getX() > MAX_ROW)
                position.setX(MIN_ROW);

            this->cannonPos = this->position;
            this->cannonPos.move(direction);
        }else {
            if (position.getY() > MAX_COL)
                position.setY(MIN_COL);
            if (position.getY() < MIN_COL)
                position.setY(MAX_COL);
            if (position.getX() < MIN_ROW)
                position.setX(MAX_ROW);
            if (position.getX() > MAX_ROW)
                position.setX(MIN_ROW);

            this->cannonPos = this->position;
        }

    }

    if (leftTrackStatus == TrackStatus::BACKWARD && rightTrackStatus == TrackStatus::BACKWARD) {
        if (hasCannon) {
            if (position.getY() > MAX_COL)
                position.setY(MIN_COL+1);
            if (position.getY() < MIN_COL)
                position.setY(MAX_COL-1);
            if (position.getX() < MIN_ROW)
                position.setX(MAX_ROW-1);
            if (position.getX() > MAX_ROW)
                position.setX(MIN_ROW+1);

            this->cannonPos = this->position;
            this->cannonPos.move(direction);
        }else {
            if (position.getY() > MAX_COL)
                position.setY(MIN_COL);
            if (position.getY() < MIN_COL)
                position.setY(MAX_COL);
            if (position.getX() < MIN_ROW)
                position.setX(MAX_ROW);
            if (position.getX() > MAX_ROW)
                position.setX(MIN_ROW);

            this->cannonPos = this->position;
        }
    }
    char cannonPosOnBoard = board.getBoardCell(cannonPos);
    char tankPosOnBoard = board.getBoardCell(position);
    if (cannonPosOnBoard == STRONG_WALL_SYMBOL || cannonPosOnBoard == WEAK_WALL_SYMBOL || isLetter(cannonPosOnBoard) ||
            tankPosOnBoard == STRONG_WALL_SYMBOL || tankPosOnBoard == WEAK_WALL_SYMBOL || isLetter(tankPosOnBoard)) {
        this->position = prevPosition;
        this->cannonPos = prevCannonPosition;
        this->direction = prevDirection;
        this->leftTrackStatus = TrackStatus::STAY;
        this->rightTrackStatus = TrackStatus::STAY;
    }

    board.setBoardCell(this->position, tankSymbol);
    gotoxy(this->position.getY()+BEGIN_BOARD_X, this->position.getX()+BEGIN_BOARD_Y);
    std::cout << tankSymbol;

    char cannon;
    switch (direction) {
        case Direction::UP: cannon = CANNON_UP_OR_DOWN_SYMBOL; break;
        case Direction::UP_LEFT: cannon = CANNON_UL_OR_DR_SYMBOL; break;
        case Direction::LEFT: cannon = CANNON_L_OR_R_SYMBOL; break;
        case Direction::DOWN_LEFT: cannon = CANNON_DL_OR_UR_SYMBOL; break;
        case Direction::DOWN: cannon = CANNON_UP_OR_DOWN_SYMBOL; break;
        case Direction::DOWN_RIGHT: cannon = CANNON_UL_OR_DR_SYMBOL; break;
        case Direction::RIGHT: cannon = CANNON_L_OR_R_SYMBOL; break;
        case Direction::UP_RIGHT: cannon = CANNON_DL_OR_UR_SYMBOL; break;
    }
    if (hasCannon) {
        board.setBoardCell(this->cannonPos, cannon);
        gotoxy(this->cannonPos.getY()+BEGIN_BOARD_X, this->cannonPos.getX()+BEGIN_BOARD_Y);
        std::cout << cannon;
    }
}

TrackStatus Tank :: getRightTrackStatus()  {
    return this -> rightTrackStatus;
}

TrackStatus Tank::getLeftTrackStatus() {
    return this -> leftTrackStatus;
}

void Tank :: setRightTrackStatus(TrackStatus status) {
    this -> rightTrackStatus = status;
}

void Tank :: setLeftTrackStatus(TrackStatus status) {
    this -> leftTrackStatus = status;
}

bool Tank::canShoot()  {
    return (this->cooldown == MIN_COOLDOWN) && hasCannon;
}

void Tank::resetCooldown() {
    cooldown = MAX_COOLDOWN;
}

void Tank::cooldownTick() {
    if (cooldown > 0) cooldown--;
}

void Tank :: markAsHit(){
    isHit = true;
};

bool Tank :: isTankHit(){
    return isHit;
    };

void Tank :: removeCannon(Board& board){
    hasCannon = false;
    this->cannonPos = this->position;
    board.setBoardCell(this->cannonPos, ' ');
    gotoxy(cannonPos.getY()+BEGIN_BOARD_X, cannonPos.getX()+BEGIN_BOARD_Y);
    cout << ' ';
};

