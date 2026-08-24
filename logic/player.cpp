#include "logic/player.h"
#include "logic/tank_movement.h"
#include "logic/tank_spawn.h"

Player::Player(const std::vector<Point>& tankPositions, int tankIND, char tankSymbol, Board& board)
    : tankIndex(tankIND), score(0), tankSymbol(tankSymbol) {
    for (const auto& pos : tankPositions) {
        myTanks.push_back(spawnTankAt(pos, board));
    }
}

void Player::changeTankFlag() {
    if (tankIndex == TANK1_FLAG) {
        if (!(myTanks[TANK2_FLAG].isTankHit()))
            tankIndex = TANK2_FLAG;
    } else if (tankIndex == TANK2_FLAG) {
        if (!(myTanks[TANK1_FLAG].isTankHit()))
            tankIndex = TANK1_FLAG;
    }
}

void Player::applyMovement(Board& board) {
    for (Tank& tank : myTanks) {
        TankMovementLogic::advance(tank, board, this->tankSymbol);
    }
}

void Player::cooldownTickAllTanks() {
    for (Tank& tank : myTanks) {
        tank.cooldownTick();
    }
}

void Player::addToScore(int addition) {
    score += addition;
}

int Player::getScore() const {
    return this->score;
}

Tank& Player::getActiveTank() {
    return this->myTanks[tankIndex];
}

std::vector<Tank>& Player::getTankVector() {
    return myTanks;
}

bool Player::allTanksDead() const {
    for (const Tank& t : myTanks) {
        if (!(t.isTankHit()))
            return false;
    }
    return true;
}
